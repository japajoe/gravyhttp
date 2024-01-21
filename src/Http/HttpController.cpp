#include "HttpController.hpp"

namespace Gravy::Http
{
    HttpController::HttpController()
    {

    }

    HttpController::HttpController(HttpRouteMapper *routeMapper)
    {

    }

    std::string HttpController::ReadContentAsString(HttpContext *context)
    {
        uint64_t contentLength = context->GetRequest()->GetContentLength();
        uint64_t bytesLeft = contentLength;
        char buffer[1024];
        std::string payload;
        
        while(bytesLeft > 0)
        {
            size_t bytesToRead = Math::Min(contentLength, 1024);
            ssize_t numBytes = context->GetStream()->Read(buffer, bytesToRead);
            if(numBytes > 0)
            {
                bytesLeft -= numBytes;
                payload += std::string(buffer, numBytes);
            }
        }

        return payload;
    }
};