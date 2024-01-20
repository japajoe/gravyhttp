#include "HttpApplication.hpp"
#include "HttpConfig.hpp"

int main(int argc, char **argv)
{
    HttpConfig config;

    if(!config.LoadFromFile("config.ini"))
    {
        Console::WriteLine("Failed to load config.ini");
        return 1;
    }

    HttpApplication application(config);
    application.Run();
}