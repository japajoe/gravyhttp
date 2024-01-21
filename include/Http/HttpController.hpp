#ifndef GRAVY_HTTPCONTROLLER_HPP
#define GRAVY_HTTPCONTROLLER_HPP

#include "HttpRouteMapper.hpp"

namespace Gravy::Http
{
    class HttpController
    {
    public:
        HttpController();
        HttpController(HttpRouteMapper *routeMapper);
    protected:
        std::string ReadContentAsString(HttpContext *context);
    };
};

#endif