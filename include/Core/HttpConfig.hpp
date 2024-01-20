#ifndef HTTPCONFIG_HPP
#define HTTPCONFIG_HPP

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

struct HttpConfig
{
public:
    std::string host;
    std::string name;
    std::string bindAddress;
    std::string publicHtml;
    std::string privateHtml;
    std::string certificatePath;
    std::string privateKeyPath;
    uint16_t port;
    uint16_t sslPort;
    bool useHttps;
    bool useHttpsForwarding;
    int32_t maxConnections;
    uint32_t maxHeaderSize;
    uint64_t maxRequestBodySize;
    uint32_t bufferSize;
    HttpConfig();
    bool LoadFromFile(const std::string &path);
private:
    template<typename T>
    bool ParseNumber(const std::string& text, T& value)
    {
        std::stringstream stream(text);
        stream >> value;
        if (stream.fail())
        {
            return false;
        }
        return true;
    }
    
    bool ParseBool(const std::string &text, bool &value)
    {
        std::string t = text;
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);

        if(t == "true" || t == "false")
        {
            if(t == "true")
                value = true;
            else
                value = false;
            return true;
        }
        return false;
    }
};

#endif