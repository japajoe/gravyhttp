#include "IndexController.hpp"
#include "HttpSettings.hpp"
#include <memory>

IndexController::IndexController(HttpRouteMapper *routeMapper)
{
    auto getHandler = [this] (HttpContext *context) {
        return OnGet(context);
    };

    routeMapper->AddGetHandler("/", HttpRequestHandlerInfo(getHandler, false));
    routeMapper->AddGetHandler("/home", HttpRequestHandlerInfo(getHandler, false));
    routeMapper->AddGetHandler("/index", HttpRequestHandlerInfo(getHandler, false));

    if(pageTemplate.GetContent().size() == 0)
        pageTemplate = HttpPageContent(HttpSettings::GetPrivateHtml() + "/template.html");

    if(pageContent.GetContent().size() == 0)
        pageContent = HttpPageContent(HttpSettings::GetPrivateHtml() + "/index.html");
}

HttpResponse IndexController::OnGet(HttpContext *context)
{
    std::string html = CreateContent();
    auto content = std::make_shared<MemoryStream>(html.data(), html.size(), true);
    HttpResponse response(HttpStatusCode::OK, HttpContentType::TextHtml, content);
    response.SetHeader("Cache-Control", "max-age=3600");
    return response;
}

std::string IndexController::CreateContent()
{
    std::string html;

    if(pageTemplate.Load() && pageContent.Load())
    {
        html = pageTemplate.GetContent();
        String::Replace(html, "$(title)", "Home - Gravy Web");
        String::Replace(html, "$(header_text)", "The HTTP/1.1 Protocol");
        String::Replace(html, "$(head)", "");

        std::string content = pageContent.GetContent();
        String::Replace(html, "$(content)", content);
    }
    else
    {
        html =
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>Gravy Web</title>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>Home</h1>\n"
        "</body>\n"
        "</html>";           
    }

    return html;        
}