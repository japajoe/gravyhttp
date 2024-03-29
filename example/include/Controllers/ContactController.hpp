#ifndef CONTACTCONTROLLER_HPP
#define CONTACTCONTROLLER_HPP

#include <GravyHttp/GravyHttp.hpp>

class ContactController : public HttpController
{
public:
    ContactController(HttpRouteMapper *routeMapper);
    HttpResponse OnGet(HttpContext *context);
private:
    HttpPageContent content;
    std::string CreateContent();
};

#endif