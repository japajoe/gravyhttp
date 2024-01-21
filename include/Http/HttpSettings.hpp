#ifndef GRAVY_HTTPSETTINGS_HPP
#define GRAVY_HTTPSETTINGS_HPP

#include "HttpConfig.hpp"

namespace Gravy::Http
{
    class HttpSettings
    {
    private:
        static HttpConfig config;
    public:
        static void LoadFromConfig(const HttpConfig &config);
        static std::string GetHost();
        static std::string GetName();
        static std::string GetBindAddress();
        static std::string GetPublicHtml();
        static std::string GetPrivateHtml();
        static std::string GetCertificatePath();
        static std::string GetPrivateKeyPath();
        static uint16_t GetPort();
        static uint16_t GetSslPort();
        static bool GetUseHttps();
        static bool GetUseHttpsForwarding();
        static int32_t GetMaxConnections();
        static uint32_t GetMaxHeaderSize();
        static uint64_t GetMaxRequestBodySize();
        static uint32_t GetBufferSize();
    };
};

#endif