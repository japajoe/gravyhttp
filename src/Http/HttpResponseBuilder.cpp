#include "HttpResponseBuilder.hpp"
#include "HttpContentType.hpp"

namespace Gravy::Http
{
    HttpResponseBuilder::HttpResponseBuilder()
    {
        
    }

    void HttpResponseBuilder::BeginHeader(HttpStatusCode status)
    {
        sb.Append("HTTP/1.1 " + std::to_string(static_cast<int>(status)) + "\r\n");
    }

    void HttpResponseBuilder::AddHeader(const std::string &key, const std::string &value)
    {
        sb.Append(key + ": " + value + "\r\n");
    }

    void HttpResponseBuilder::AddHeader(const std::string &key, size_t value)
    {
        sb.Append(key + ": " + std::to_string(value) + "\r\n");
    }

    void HttpResponseBuilder::AddHeader(const std::string &key, ssize_t value)
    {
        sb.Append(key + ": " + std::to_string(value) + "\r\n");
    }

    void HttpResponseBuilder::AddHeader(const std::string &key, float value)
    {
        sb.Append(key + ": " + std::to_string(value) + "\r\n");
    }

    void HttpResponseBuilder::AddHeader(const std::string &key, double value)
    {
        sb.Append(key + ": " + std::to_string(value) + "\r\n");
    }

    void HttpResponseBuilder::AddString(const std::string &str)
    {
        sb.Append(str);
    }

    void HttpResponseBuilder::EndHeader()
    {
        sb.Append("\r\n");
    }

    void HttpResponseBuilder::Clear()
    {
        sb.Clear();
    }

    std::string HttpResponseBuilder::ToString() const
    {
        return sb.ToString();
    }
};