#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <unordered_map>
#include "HttpMethod.hpp"
#include "HttpContentType.hpp"
#include "HttpHeaders.hpp"

class HttpRequest
{
private:
    HttpMethod method;
    std::string path;
    std::string pathRaw;
    HttpHeaders headers;
    uint64_t contentLength;
    HttpContentType contentType;
    HttpCharSet charSet;
public:
    HttpRequest();
    static bool TryParse(const std::string &header, HttpRequest &request);
    HttpHeaders GetHeaders() const;
    bool GetHeader(const std::string &key, std::string &value);
    void SetHeader(const std::string &key, const std::string &value);
    std::string GetPath() const;
    std::string GetPathRaw() const;
    HttpMethod GetMethod() const;
    uint64_t GetContentLength() const;
    HttpContentType GetContentType() const;
    HttpCharSet GetCharSet() const;
};

#endif