#ifndef GRAVY_HTTPMETHOD_HPP
#define GRAVY_HTTPMETHOD_HPP

namespace Gravy::Http
{
    enum class HttpMethod
    {
        Connect,
        Delete,
        Get,
        Head,
        Options,
        Patch,
        Post,
        Put,
        Trace,
        Unknown
    };
};

#endif