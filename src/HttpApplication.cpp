#include "HttpApplication.hpp"
#include "HttpSettings.hpp"
#include "IndexController.hpp"
#include "ContactController.hpp"
#include "ExampleController.hpp"
#include "NotFoundController.hpp"

static HttpServer *serverInstance = nullptr;
static void OnHandleSignal(int signalnum);

HttpApplication::HttpApplication(const HttpConfig &config)
{
    server = std::make_unique<HttpServer>(config);
    
    auto requestHandler = [this] (HttpContext *context) {
        return OnHttpRequest(context);
    };

    server->SetRequestHandler(requestHandler);
    
    serverInstance = server.get();

    Signal::Register(SignalType::SigInt, OnHandleSignal);
#ifndef _WIN32 //Windows does not have this signal
    Signal::Register(SignalType::SigPipe, OnHandleSignal);
#endif

    routeMapper = std::make_unique<HttpRouteMapper>();

    controllers.push_back(std::make_unique<IndexController>(routeMapper.get()));
    controllers.push_back(std::make_unique<ExampleController>(routeMapper.get()));
    controllers.push_back(std::make_unique<ContactController>(routeMapper.get()));
    controllers.push_back(std::make_unique<NotFoundController>(routeMapper.get()));
}

void HttpApplication::Run()
{
    server->Run();
}

HttpResponse HttpApplication::OnHttpRequest(HttpContext *context)
{
    auto route = routeMapper->GetRoute(context->GetRequest()->GetPath(), false);

    if(route)
        return route->GetResponse(context);

    static std::string public_html = HttpSettings::GetPublicHtml();
    std::string filepath = public_html + context->GetRequest()->GetPath();

    if(File::Exists(filepath))
    {
        if(File::IsWithinDirectory(filepath, public_html))
        {
            std::string body = File::ReadAllText(filepath);
            HttpContentType contentType = HttpContent::GetTypeFromFileExtension(filepath);
            auto content = std::make_shared<MemoryStream>((void*)body.data(), body.size(), true);      
            HttpResponse response(HttpStatusCode::OK, contentType, content);
            response.SetHeader("Cache-Control", "max-age=3600");
            return response;
        }
    }
    
    route = routeMapper->GetRoute("/404", true);
    
    if(route)
        return route->GetResponse(context);

    return HttpResponse(HttpStatusCode::NotFound); 
}

void OnHandleSignal(int signalnum)
{
    SignalType type = static_cast<SignalType>(signalnum);

    switch(type)
    {
        case SignalType::SigInt:
        {
            if(serverInstance)
                serverInstance->Close();
            exit(0);
            break;
        }
    #ifndef _WIN32 //Windows does not have this signal
        case SignalType::SigPipe:
        {
            break;
        }
    #endif
        default:
            break;
    }
}