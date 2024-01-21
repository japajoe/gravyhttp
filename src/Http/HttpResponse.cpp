#include "HttpResponse.hpp"
#include "HttpResponseBuilder.hpp"

namespace Gravy::Http
{
    HttpResponse::HttpResponse()
    {
        this->status = HttpStatusCode::OK;
        this->contentType = HttpContentType::TextPlain;
        this->content = nullptr;
        this->contentLength = 0;
    }

    HttpResponse::HttpResponse(HttpStatusCode status)
    {
        this->status = status;
        this->contentType = HttpContentType::TextPlain;
        this->content = nullptr;
        this->contentLength = 0;
    }

    HttpResponse::HttpResponse(HttpStatusCode status, HttpContentType contentType)
    {
        this->status = status;
        this->contentType = contentType;
        this->content = nullptr;
        this->contentLength = 0;
    }

    HttpResponse::HttpResponse(HttpStatusCode status, HttpContentType contentType, const std::shared_ptr<Stream> &content)
    {
        this->status = status;
        this->contentType = contentType;
        this->content = content;
        this->contentLength = this->content->GetSize();
    }

    HttpStatusCode HttpResponse::GetStatusCode() const
    {
        return status;
    }

    void HttpResponse::SetStatusCode(HttpStatusCode status)
    {
        this->status = status;
    }

    HttpContentType HttpResponse::GetContentType() const
    {
        return contentType;
    }

    void HttpResponse::SetContentType(HttpContentType contentType)
    {
        this->contentType = contentType;
    }

    uint64_t HttpResponse::GetContentLength() const
    {
        return contentLength;
    }

    HttpHeaders *HttpResponse::GetHeaders()
    {
        return &headers;
    }

    bool HttpResponse::GetHeader(const std::string &key, std::string &value)
    {
        if(headers.count(key) > 0)
        {
            value = headers[key];
            return true;
        }

        return false;
    }

    void HttpResponse::SetHeader(const std::string &key, const std::string &value)
    {
        headers[key] = value;
    }

    Stream *HttpResponse::GetContent()
    {
        if(content)
            return content.get();
        return nullptr;
    }

    std::string HttpResponse::ToString()
    {
        HttpResponseBuilder builder;
        builder.BeginHeader(status);

        for(auto &item : headers)
        {
            builder.AddHeader(item.first, item.second);
        }

        if(content != nullptr)
        {
            if(content->GetSize() > 0)
            {
                std::string type = HttpContent::GetStringFromContentType(this->contentType);
                builder.AddHeader("Content-Type", type);
                builder.AddHeader("Content-Length", content->GetSize());
            }
            else
            {
                //Needed for persistent connections or browser keeps waiting indefinitely
                builder.AddHeader("Content-Length", static_cast<int64_t>(0));
            }
        }
        else
        {
            //Needed for persistent connections or browser keeps waiting indefinitely
            builder.AddHeader("Content-Length", static_cast<int64_t>(0));
        }

        builder.EndHeader();
        return builder.ToString();
    }
};