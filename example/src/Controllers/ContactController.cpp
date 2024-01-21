#include "ContactController.hpp"
#include <memory>

ContactController::ContactController(HttpRouteMapper *routeMapper)
{
    auto getHandler = [this] (HttpContext *context) {
        return OnGet(context);
    };

    routeMapper->AddGetHandler("/contact", HttpRequestHandlerInfo(getHandler, false));

    if(content.GetContent().size() == 0)
        content = HttpPageContent(HttpSettings::GetPrivateHtml() + "/template.html");
}

HttpResponse ContactController::OnGet(HttpContext *context)
{
    std::string html = CreateContent();
    auto content = std::make_shared<MemoryStream>(html.data(), html.size(), true);
    HttpResponse response(HttpStatusCode::OK, HttpContentType::TextHtml, content);
    response.SetHeader("Cache-Control", "max-age=3600");
    return response;
}

std::string ContactController::CreateContent()
{
    std::string html;

    if(content.Load())
    {
        html = content.GetContent();
        String::Replace(html, "$(title)", "Contact - Gravy Web");
        String::Replace(html, "$(header_text)", "Contact");
        String::Replace(html, "$(head)", "");
        String::Replace(html, "$(content)", "<p>Contact me on <a href=\"https://github.com/japajoe\" target=\"blank\">GitHub</a></p>");
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
        "    <h1>Contact</h1>\n"
        "</body>\n"
        "</html>";           
    }

    return html;
}