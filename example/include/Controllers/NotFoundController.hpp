#ifndef NOTFOUNDCONTROLLER_HPP
#define NOTFOUNDCONTROLLER_HPP

#include <GravyHttp/GravyHttp.hpp>

class NotFoundController : public HttpController
{
public:
    NotFoundController(HttpRouteMapper *routeMapper);
    HttpResponse OnGet(HttpContext *context);
private:
    HttpPageContent content;
    std::string CreateContent();
};

#endif