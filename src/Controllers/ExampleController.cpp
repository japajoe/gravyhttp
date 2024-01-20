#include "ExampleController.hpp"
#include "HttpSettings.hpp"
#include <memory>
#include <regex>

ExampleController::ExampleController(HttpRouteMapper *routeMapper)
{
    numRequests = 0;

    auto getHandler = [this] (HttpContext *context) {
        return OnGet(context);
    };

    auto postHandler = [this] (HttpContext *context) {
        return OnPost(context);
    };       

    routeMapper->AddGetHandler("/example", HttpRequestHandlerInfo(getHandler, false));
    routeMapper->AddPostHandler("/api/v1/example", HttpRequestHandlerInfo(postHandler, false));

    if(pageContent.GetContent().size() == 0)
        pageContent = HttpPageContent(HttpSettings::GetPrivateHtml() + "/template.html");
}

HttpResponse ExampleController::OnGet(HttpContext *context)
{
    std::string html = CreateContent();
    auto content = std::make_shared<MemoryStream>(html.data(), html.size(), true);
    HttpResponse response(HttpStatusCode::OK, HttpContentType::TextHtml, content);
    response.SetHeader("Cache-Control", "max-age=3600");
    return response;
}

HttpResponse ExampleController::OnPost(HttpContext *context)
{
    //Content must be application/json
    if(context->GetRequest()->GetContentType() != HttpContentType::ApplicationJson)
        return HttpResponse(HttpStatusCode::BadRequest);

    //Content length must be greater than 0
    if(context->GetRequest()->GetContentLength() == 0)
        return HttpResponse(HttpStatusCode::BadRequest);

    std::string json = ReadContentAsString(context);

    //Check if json has a valid anti forgery token
    if(!HasValidToken(json))
        return HttpResponse(HttpStatusCode::Unauthorized);
    
    //Prepare response content
    JsonBuilder jb;
    jb.AddProperty("Message", "Thank you!");
    jb.AddProperty("NumberOfRequests", ++numRequests);
    std::string payload = jb.ToString();
    
    auto content = std::make_shared<MemoryStream>(payload.data(), payload.size(), true);
    HttpResponse response(HttpStatusCode::OK, HttpContentType::ApplicationJson, content);
    return response;
}   

std::string ExampleController::CreateContent()
{
    std::string html;

    if(pageContent.Load())
    {
        html = pageContent.GetContent();
        String::Replace(html, "$(title)", "Example - Gravy Web");
        String::Replace(html, "$(header_text)", "Example");
        String::Replace(html, "$(head)", "<script src=\"/api.js\"></script>");

        DateTime expires = DateTime::Now().AddMinutes(1);

        std::string token = AntiForgery::GenerateToken(expires).ToString();

        StringBuilder sb;
        sb.AppendLine("<p>Click the button to send a POST request. If you get an error status 401 then it means your request token has expired. Refresh the page and try again.<p>");
        sb.AppendLine("\t\t<p><button class=\"btn-style\" type=\"button\" id=\"buttonSend\">Send</button></p>");
        sb.AppendLine("\t\t<p id=\"responseArea\"></p>");
        sb.AppendLine("\t\t<input type=\"hidden\" name=\"csrf-token\" value=\"" + token + "\">");

        String::Replace(html, "$(content)", sb.ToString());
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
        "    <h1>Example</h1>\n"
        "</body>\n"
        "</html>";           
    }

    return html;    
}

bool ExampleController::HasValidToken(const std::string &json)
{
    static std::regex tokenRegex("\"token\"\\s*:\\s*\"([^\"]+)\"");
    std::smatch match;

    if (std::regex_search(json, match, tokenRegex)) 
    {
        std::string token = match[1];
        Guid guid;
        if(Guid::TryParse(token, guid))
        {
            if(AntiForgery::IsValidToken(guid))
            {
                return true;
            }
        }            
    }

    return false;
}