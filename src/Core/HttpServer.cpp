#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpSettings.hpp"
#include <future>
#include <iostream>
#include <vector>

HttpServer::HttpServer(const HttpConfig &config)
{
    if(config.useHttps)
    {
        if(!File::Exists(config.certificatePath))
        {
            Console::WriteError("Certificate file not found: " + config.certificatePath);
            exit(1);
        }

        if(!File::Exists(config.privateKeyPath))
        {
            Console::WriteError("Private key file not found: " + config.privateKeyPath);
            exit(1);
        }
    }

    if(!Directory::Exists(config.privateHtml))
    {
        Console::WriteError("Private html directory does not exist: " + config.privateHtml);
        exit(1);
    }

    if(!Directory::Exists(config.publicHtml))
    {
        Console::WriteError("Public html directory does not exist: " + config.publicHtml);
        exit(1);
    }

    HttpSettings::LoadFromConfig(config);

    this->requestHandler = nullptr;
    this->bindAddress = config.bindAddress;
    this->port = config.port;
    this->portSsl = config.sslPort;
    this->backlog = config.maxConnections <= 0 ? 2147483647 : config.maxConnections;
    this->maxHeaderSize = config.maxHeaderSize;
    isRunning = false;
}

void HttpServer::SetRequestHandler(const HttpRequestHandler &requestHandler)
{
    this->requestHandler = requestHandler;
}

void HttpServer::Run()
{
    if(isRunning)
        return;

    isRunning = true;    

    try
    {
        std::string certificate = HttpSettings::GetCertificatePath();
        std::string key = HttpSettings::GetPrivateKeyPath();
        sslContext = std::make_unique<SslContext>(certificate, key);
    }
    catch(const SslException &ex)
    {
        Console::WriteError(ex.what());
        exit(1);
    }

    if(port > 0 && portSsl > 0)
    {
        auto task1 = std::async(std::launch::async, &HttpServer::HandleHttp, this);
        auto task2 = std::async(std::launch::async, &HttpServer::HandleHttps, this);
    }
    else
    {
        if(port > 0)
        {
            auto task1 = std::async(std::launch::async, &HttpServer::HandleHttp, this);
        }
        if(portSsl > 0)
        {
            auto task2 = std::async(std::launch::async, &HttpServer::HandleHttps, this);
        }
    }
}

void HttpServer::Close()
{
    if(isRunning)
    {
        isRunning = false;

        if(httpListener)
            httpListener->Close();

        if(httpsListener)
            httpsListener->Close();

        Console::WriteLine("Stopping server");
    }
}

void HttpServer::HandleHttp()
{
    httpListener = std::make_unique<TcpListener>(bindAddress, port, backlog);

    if(!httpListener->Start())
    {
        httpListener->Close();
        return;
    }

    Console::WriteLog("Listening on http://localhost:" + std::to_string(port));

    while(isRunning)
    {
        TcpClient client;

        if(httpListener->Accept(client))
        {
            Socket socket(client.GetSocket());
            NetworkStream stream(socket);
            auto result = std::async(std::launch::async, &HttpServer::HandleConnection, this, stream);
        }
    }
}

void HttpServer::HandleHttps()
{
    httpsListener = std::make_unique<TcpListener>(bindAddress, portSsl, backlog);
    
    if(!httpsListener->Start())
    {
        httpsListener->Close();
        return;
    }

    Console::WriteLog("Listening on https://localhost:" + std::to_string(portSsl));

    while(isRunning)
    {
        TcpClient client;

        if(httpsListener->Accept(client))
        {
            try
            {
                Socket socket(client.GetSocket());
                SslStream ssl = SslStream(&socket, sslContext.get());
                NetworkStream stream(socket, ssl);
                auto result = std::async(std::launch::async, &HttpServer::HandleConnection, this, stream);
            }
            catch(const SslException &ex)
            {
                client.Close();
            }   
        }
    }
}

void HttpServer::HandleConnection(NetworkStream stream)
{
loop_handle_connection:
    std::string header;
    int32_t headerSize = ReadHeader(&stream, header);        

    if(headerSize <= 0 || headerSize > maxHeaderSize)
    {
        if(headerSize > 0)
        {
            HttpResponse response(HttpStatusCode::RequestHeaderFieldsTooLarge);
            Send(&stream, &response);
        }

        stream.Close();
        return;
    }

    HttpRequest request;

    if(!HttpRequest::TryParse(header, request))
    {
        HttpResponse response(HttpStatusCode::BadRequest);
        Send(&stream, &response);
        stream.Close();
        return;
    }

    if(!stream.IsSecure() && portSsl > 0)
    {
        std::string upgrade;

        if(request.GetHeader("Upgrade-Insecure-Requests", upgrade))
        {
            if(upgrade == "1")
            {
                std::string location = "https://localhost:" + std::to_string(portSsl) + request.GetPath();
                HttpResponse response(HttpStatusCode::MovedPermanently);
                response.SetHeader("Location", location);
                response.SetHeader("Connection", "close");
                Send(&stream, &response);
                stream.Close();
                return;
            }
        }
    }

    if (requestHandler)
    {
        HttpContext context(&stream, &request);
        HttpResponse response = requestHandler(&context);
        Send(&stream, &response);
    }
    else
    {
        HttpResponse response = HttpResponse(HttpStatusCode::OK);
        Send(&stream, &response);
    }

    // Keeping connection alive gives issues I've yet to solve
    static bool ignoreKeepAlive = true;
    std::string connection;

    if(request.GetHeader("Connection", connection) && !ignoreKeepAlive)
    {
        // Using a goto saves us from having to wrap everything in this method into a big while(true)
        if(connection == "keep-alive")
            goto loop_handle_connection;
    }

    stream.Close();
}

uint32_t HttpServer::ReadHeader(NetworkStream *stream, std::string &header)
{
    std::vector<char> headerBuilder;
    unsigned char prevByte = 0;
    unsigned char currentByte = 0;
    int curByte = 0;
    int headerSize = 0;

    while ((curByte = stream->ReadByte()) != -1)
    {
        currentByte = static_cast<unsigned char>(curByte);
        headerBuilder.push_back(static_cast<char>(currentByte));
        headerSize++;

        if(headerSize > maxHeaderSize)
            break;

        if (prevByte == '\r' && currentByte == '\n')
        {
            std::string str(headerBuilder.begin(), headerBuilder.end());

            if(String::EndsWith(str, "\r\n\r\n"))
            {
                break;
            }
        }

        prevByte = currentByte;
    }

    header = std::string(headerBuilder.begin(), headerBuilder.end());
    return headerSize;
}

void HttpServer::Send(NetworkStream *stream, HttpResponse *response)
{
    if(!response || !stream)
        return;

    std::string responseHeader = response->ToString();
    MemoryStream headerStream((void*)responseHeader.data(), responseHeader.size(), false);
    
    if(Send(stream, &headerStream))
    {
        if(response->GetContent() != nullptr)
        {
            Send(stream, response->GetContent());
            response->GetContent()->Close();
        }
    }
}

bool HttpServer::Send(NetworkStream *stream, Stream *data)
{
    if(data == nullptr)
        return false;

    if(data->GetSize() == 0)
        return false;

    const size_t chunkSize = 1024;
    unsigned char buffer[1024];
    std::memset(buffer, 0, 1024);
    size_t dataSize = data->GetSize();

    for (size_t offset = 0; offset < dataSize; offset += chunkSize) 
    {
        size_t remaining = dataSize - offset;
        size_t currentChunkSize = (remaining > chunkSize) ? chunkSize : remaining;

        // Copy the current chunk of data into the buffer
        data->Read(buffer, currentChunkSize);

        if(stream->Write(buffer, currentChunkSize) <= 0)
            return false;
    }

    return true;
}