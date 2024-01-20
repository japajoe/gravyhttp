#ifndef INDEXCONTROLLER_HPP
#define INDEXCONTROLLER_HPP

#include "HttpController.hpp"
#include "HttpPageContent.hpp"

class IndexController : public HttpController
{
public:
    IndexController(HttpRouteMapper *routeMapper);
    HttpResponse OnGet(HttpContext *context);
private:
    HttpPageContent pageTemplate;
    HttpPageContent pageContent;
    std::string CreateContent();
};

#endif