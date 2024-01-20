#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#define GRAVY_USE_OPENSSL
#include <Gravy/Gravy.hpp>
#include "HttpContext.hpp"
#include "HttpResponse.hpp"
#include "HttpConfig.hpp"
#include <memory>
#include <atomic>
#include <functional>

using HttpRequestHandler = std::function<HttpResponse(HttpContext*)>;

class HttpServer
{
private:
    HttpRequestHandler requestHandler;
    std::unique_ptr<TcpListener> httpListener;
    std::unique_ptr<TcpListener> httpsListener;
    std::unique_ptr<SslContext> sslContext;
    std::atomic<bool> isRunning;
    std::string bindAddress;
    uint16_t port;
    uint16_t portSsl;
    int32_t backlog;
    uint32_t maxHeaderSize;
    void HandleHttp();
    void HandleHttps();
    void HandleConnection(NetworkStream stream);
    uint32_t ReadHeader(NetworkStream *stream, std::string &header);
    void Send(NetworkStream *stream, HttpResponse *response);
    bool Send(NetworkStream *stream, Stream *data);
public:
    HttpServer(const HttpConfig &config);
    void SetRequestHandler(const HttpRequestHandler &requestHandler);
    void Run();
    void Close();
};

#endif