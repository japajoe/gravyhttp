#ifndef GRAVY_HTTPRESPONSEBUILDER_HPP
#define GRAVY_HTTPRESPONSEBUILDER_HPP

#include "HttpResponse.hpp"
#include <Gravy/Gravy.hpp>
#include <string>
#include <exception>

namespace Gravy::Http
{
    class HttpResponseBuilder
    {
    private:
        StringBuilder sb;
    public:
        HttpResponseBuilder();
        void BeginHeader(HttpStatusCode status);
        void AddHeader(const std::string &key, const std::string &value);
        void AddHeader(const std::string &key, size_t value);
        void AddHeader(const std::string &key, ssize_t value);
        void AddHeader(const std::string &key, float value);
        void AddHeader(const std::string &key, double value);
        void AddString(const std::string &str);
        void EndHeader();
        void Clear();
        std::string ToString() const;
    };

    class HttpResponseBuilderException : public std::exception 
    {
    public:
        HttpResponseBuilderException(const char* message) : message_(message) {}

        const char* what() const noexcept override 
        {
            return message_.c_str();
        }
    private:
        std::string message_;
    };
};

#endif