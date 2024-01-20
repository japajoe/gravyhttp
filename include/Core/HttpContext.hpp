#ifndef HTTPCONTEXT_HPP
#define HTTPCONTEXT_HPP

#include <Gravy/Gravy.hpp>
#include "HttpRequest.hpp"

class HttpContext
{
private:
    NetworkStream *stream;
    HttpRequest *request;
public:
    HttpContext()
    {

    }
    
    HttpContext(NetworkStream *stream, HttpRequest *request)
    {
        this->stream = stream;
        this->request = request;
    }

    NetworkStream *GetStream()
    {
        return stream;
    }

    HttpRequest *GetRequest()
    {
        return request;
    }
};

#endif