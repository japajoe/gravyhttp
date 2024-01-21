#ifndef EXAMPLECONTROLLER_HPP
#define EXAMPLECONTROLLER_HPP

#include <GravyHttp/GravyHttp.hpp>

class ExampleController : public HttpController
{
public:
    ExampleController(HttpRouteMapper *routeMapper);
    HttpResponse OnGet(HttpContext *context);
    HttpResponse OnPost(HttpContext *context);
private:
    int numRequests;
    HttpPageContent pageContent;
    std::string CreateContent();
    bool HasValidToken(const std::string &json);
};

#endif