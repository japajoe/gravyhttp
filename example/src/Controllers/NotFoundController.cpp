#include "NotFoundController.hpp"
#include <memory>

NotFoundController::NotFoundController(HttpRouteMapper *routeMapper)
{
    auto getHandler = [this] (HttpContext *context) {
        return OnGet(context);
    };

    routeMapper->AddGetHandler("/404", HttpRequestHandlerInfo(getHandler, true));

    if(content.GetContent().size() == 0)
        content = HttpPageContent(HttpSettings::GetPrivateHtml() + "/template.html");
}

HttpResponse NotFoundController::OnGet(HttpContext *context)
{
    std::string html = CreateContent();
    auto content = std::make_shared<MemoryStream>(html.data(), html.size(), true);
    HttpResponse response(HttpStatusCode::NotFound, HttpContentType::TextHtml, content);
    response.SetHeader("Cache-Control", "max-age=3600");
    return response;
}

std::string NotFoundController::CreateContent()
{
    bool result = content.Load();

    std::string html = "Hello world";

    if(result)
    {
        html = content.GetContent();
        String::Replace(html, "$(title)", "Not Found - Gravy Web");
        String::Replace(html, "$(header_text)", "Error 404");
        String::Replace(html, "$(head)", "");
        String::Replace(html, "$(content)", "<p>The requested document was not found</p>");
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
        "    <h1>Whoops 404 Not Found</h1>\n"
        "</body>\n"
        "</html>";        
    }

    return html;
}