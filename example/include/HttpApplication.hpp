#ifndef HTTPAPPLICATION_HPP
#define HTTPAPPLICATION_HPP

#include <Gravy/Gravy.hpp>
#include <GravyHttp/GravyHttp.hpp>
#include <vector>
#include <memory>

class HttpApplication
{
private:
    std::unique_ptr<HttpServer> server;
    std::unique_ptr<HttpRouteMapper> routeMapper;
    std::vector<std::unique_ptr<HttpController>> controllers;
    HttpResponse OnHttpRequest(HttpContext *context);
public:
    HttpApplication(const HttpConfig &config);
    void Run();
};

#endif