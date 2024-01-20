#ifndef HTTPCONTROLLER_HPP
#define HTTPCONTROLLER_HPP

#include "HttpRouteMapper.hpp"

class HttpController
{
public:
    HttpController();
    HttpController(HttpRouteMapper *routeMapper);
protected:
    std::string ReadContentAsString(HttpContext *context);
};

#endif