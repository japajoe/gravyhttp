#include "HttpRequest.hpp"
#include <Gravy/Gravy.hpp>

namespace Gravy::Http
{
    static std::unordered_map<std::string,HttpMethod> httpMethodTable;

    static void CreateHttpMethodTable()
    {
        if(httpMethodTable.size() > 0)
            return;
        
        httpMethodTable["connect"] = HttpMethod::Connect;
        httpMethodTable["delete"] = HttpMethod::Delete;
        httpMethodTable["get"] = HttpMethod::Get;
        httpMethodTable["head"] = HttpMethod::Head;
        httpMethodTable["options"] = HttpMethod::Options;
        httpMethodTable["patch"] = HttpMethod::Patch;
        httpMethodTable["post"] = HttpMethod::Post;
        httpMethodTable["put"] = HttpMethod::Put;
        httpMethodTable["trace"] = HttpMethod::Trace;
    }

    HttpRequest::HttpRequest()
    {
        CreateHttpMethodTable();
    }

    bool HttpRequest::TryParse(const std::string &header, HttpRequest &request)
    {
        std::string h = header;
        String::Replace(h, "\r", "");
        auto lines = String::Split(h, '\n');

        if(lines.size() == 0)
            return false;

        auto methodParts = String::Split(lines[0], ' ');

        if(methodParts.size() < 3)
            return false;

        std::string method = String::ToLower(methodParts[0]);
        std::string path = methodParts[1];
        std::string protocol = methodParts[2];

        if(httpMethodTable.count(method) > 0)
            request.method = httpMethodTable[method];
        else
            request.method = HttpMethod::Unknown;

        request.path = path;
        request.pathRaw = path;

        if(String::Contains(path, "?"))
        {
            auto components = String::Split(path, '?');
            request.path = components[0];
        }

        for(size_t i = 1; i < lines.size(); ++i)
        {
            auto headerParts = String::Split(lines[i], ':');
            
            if(headerParts.size() < 2)
                continue;
            
            auto key = String::Trim(headerParts[0]);
            auto value = String::Trim(headerParts[1]);

            if(key.size() == 0 || value.size() == 0)
                continue;
            
            request.headers[key] = value;
        }

        std::string strContentLength;

        if(request.GetHeader("Content-Length", strContentLength))
        {
            uint64_t contentLength = 0;

            if(String::TryParseUInt64(strContentLength, contentLength))
                request.contentLength = contentLength;
            else
                request.contentLength = 0;
        }
        else
        {
            request.contentLength = 0;
        }

        std::string strContentType;

        if(request.GetHeader("Content-Type", strContentType))
        {
            HttpContentType contentType;
            HttpCharSet charSet;
            HttpContent::GetTypeFromHeader(strContentType, contentType, charSet);
            request.contentType = contentType;
            request.charSet = charSet;
        }
        else
        {
            request.contentType = HttpContentType::Any;
            request.charSet = HttpCharSet::UTF8;
        }

        return true;
    }

    HttpHeaders HttpRequest::GetHeaders() const
    {
        return headers;
    }

    bool HttpRequest::GetHeader(const std::string &key, std::string &value)
    {
        if(headers.count(key) > 0)
        {
            value = headers[key];
            return true;
        }

        return false;
    }

    void HttpRequest::SetHeader(const std::string &key, const std::string &value)
    {
        headers[key] = value;
    }

    std::string HttpRequest::GetPath() const
    {
        return path;
    }

    std::string HttpRequest::GetPathRaw() const
    {
        return pathRaw;
    }

    HttpMethod HttpRequest::GetMethod() const
    {
        return method;
    }

    uint64_t HttpRequest::GetContentLength() const
    {
        return contentLength;
    }

    HttpContentType HttpRequest::GetContentType() const
    {
        return contentType;
    }

    HttpCharSet HttpRequest::GetCharSet() const
    {
        return charSet;
    }
};