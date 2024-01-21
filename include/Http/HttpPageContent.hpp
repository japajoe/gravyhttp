#ifndef GRAVY_HTTPPAGECONTENT_HPP
#define GRAVY_HTTPPAGECONTENT_HPP

#include <string>
#include <unordered_map>
#include <Gravy/Gravy.hpp>

namespace Gravy::Http
{
    class HttpPageContent
    {
    private:
        std::string path;
        std::string content;
        FileTimeInfo fileInfo;
    public:
        HttpPageContent();
        HttpPageContent(const std::string &path);
        bool Load();
        std::string GetPath() const;
        std::string GetContent() const;
        FileTimeInfo GetFileInfo() const;
    };
};
#endif