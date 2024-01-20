#include "HttpRouteMapper.hpp"
#include "HttpSettings.hpp"
#include <string>

HttpRequestHandlerInfo::HttpRequestHandlerInfo(const HttpRequestHandler &handler, bool isInternal)
{
    this->handler = handler;
    this->isInternal = isInternal;
}

HttpResponse HttpRequestHandlerInfo::GetResponse(HttpContext *context)
{
    if(context->GetRequest()->GetMethod() != method)
        return HttpResponse(HttpStatusCode::MethodNotAllowed);

    if(handler)
        return handler(context);
    
    return HttpResponse(HttpStatusCode::NotImplemented);
}

#include <iostream>

HttpRequestHandlerInfo *HttpRouteMapper::GetRoute(const std::string &url, bool allowInternal)
{
    for (auto& route : routes) 
    {
        std::smatch match;
        if (std::regex_match(url, match, route.first))
        {
            // Not sure yet about this...
            // std::unordered_map<std::string, std::string> params;
            // for (size_t i = 1; i < match.size(); ++i) 
            // {
            //     params["$" + std::to_string(i)] = match[i].str();
            // }

            if(route.second.isInternal)
            {
                if(allowInternal)
                    return &route.second;
                return nullptr;
            }

            return &route.second;
        }
    }

    return nullptr;
}

void HttpRouteMapper::AddConnectHandler(const std::string &route, const HttpRequestHandlerInfo &handlerInfo)
{
    const_cast<HttpRequestHandlerInfo*>(&handlerInfo)->method = HttpMethod::Connect;
    routes.emplace_back(std::regex(route), handlerInfo);
}

void HttpRouteMapper::AddDeleteHandler(const std::string &route, const HttpRequestHandlerInfo &handlerInfo)
{
    const_cast<HttpRequestHandlerInfo*>(&handlerInfo)->method = HttpMethod::Delete;
    routes.emplace_back(std::regex(route), handlerInfo);
}

void HttpRouteMapper::AddGetHandler(const std::string &route, const HttpRequestHandlerInfo &handlerInfo)
{
    const_cast<HttpRequestHandlerInfo*>(&handlerInfo)->method = HttpMethod::Get;
    routes.emplace_back(std::regex(route), handlerInfo);
}

void HttpRouteMapper::AddHeadHandler(const std::string &route, const HttpRequestHandlerInfo &handlerInfo)
{
    const_cast<HttpRequestHandlerInfo*>(&handlerInfo)->method = HttpMethod::Head;
    routes.emplace_back(std::regex(route), handlerInfo);
}

void HttpRouteMapper::AddOptionsHandler(const std::string &route, const HttpRequestHandlerInfo &handlerInfo)
{
    const_cast<HttpRequestHandlerInfo*>(&handlerInfo)->method = HttpMethod::Options;
    routes.emplace_back(std::regex(route), handlerInfo);
}

void HttpRouteMapper::AddPatchHandler(const std::string &route, const HttpRequestHandlerInfo &handlerInfo)
{
    const_cast<HttpRequestHandlerInfo*>(&handlerInfo)->method = HttpMethod::Patch;
    routes.emplace_back(std::regex(route), handlerInfo);
}

void HttpRouteMapper::AddPostHandler(const std::string &route, const HttpRequestHandlerInfo &handlerInfo)
{
    const_cast<HttpRequestHandlerInfo*>(&handlerInfo)->method = HttpMethod::Post;
    routes.emplace_back(std::regex(route), handlerInfo);
}

void HttpRouteMapper::AddPutHandler(const std::string &route, const HttpRequestHandlerInfo &handlerInfo)
{
    const_cast<HttpRequestHandlerInfo*>(&handlerInfo)->method = HttpMethod::Put;
    routes.emplace_back(std::regex(route), handlerInfo);
}

void HttpRouteMapper::AddTraceHandler(const std::string &route, const HttpRequestHandlerInfo &handlerInfo)
{
    const_cast<HttpRequestHandlerInfo*>(&handlerInfo)->method = HttpMethod::Trace;
    routes.emplace_back(std::regex(route), handlerInfo);
}
