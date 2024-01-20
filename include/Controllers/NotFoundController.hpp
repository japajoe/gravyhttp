#ifndef NOTFOUNDCONTROLLER_HPP
#define NOTFOUNDCONTROLLER_HPP

#include "HttpController.hpp"
#include "HttpPageContent.hpp"

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