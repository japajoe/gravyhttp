#ifndef GRAVY_HTTPRESPONSE_HPP
#define GRAVY_HTTPRESPONSE_HPP

#include <Gravy/Gravy.hpp>
#include <memory>
#include "HttpStatusCode.hpp"
#include "HttpContentType.hpp"
#include "HttpHeaders.hpp"

namespace Gravy::Http
{
    class HttpResponse
    {
    private:
        HttpStatusCode status;
        HttpContentType contentType;
        uint64_t contentLength;
        HttpHeaders headers;
        std::shared_ptr<Stream> content;
    public:
        HttpResponse();
        HttpResponse(HttpStatusCode status);
        HttpResponse(HttpStatusCode status, HttpContentType contentType);
        HttpResponse(HttpStatusCode status, HttpContentType contentType, const std::shared_ptr<Stream> &content);
        HttpStatusCode GetStatusCode() const;
        void SetStatusCode(HttpStatusCode status);
        HttpContentType GetContentType() const;
        void SetContentType(HttpContentType contentType);
        uint64_t GetContentLength() const;
        HttpHeaders *GetHeaders();
        bool GetHeader(const std::string &key, std::string &value);
        void SetHeader(const std::string &key, const std::string &value);
        Stream *GetContent();
        std::string ToString();
    };
};

#endif