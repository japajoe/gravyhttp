#include "HttpPageContent.hpp"

namespace Gravy::Http
{
    HttpPageContent::HttpPageContent()
    {
        this->path.clear();
        this->content.clear();
        this->fileInfo = FileTimeInfo();
    }

    HttpPageContent::HttpPageContent(const std::string &path)
    {
        this->path = path;
        this->content.clear();
        this->fileInfo = FileTimeInfo();
    }

    bool HttpPageContent::Load()
    {
        if(!File::Exists(path))
            return false;

        FileInfo info(path);
        FileTimeInfo fileTimeInfo(info);
        
        if(fileInfo.lastWriteTime != fileTimeInfo.creationTime)
        {
            content = File::ReadAllText(path);
            fileInfo = fileTimeInfo;
        }

        return true;
    }

    std::string HttpPageContent::GetPath() const
    {
        return path;
    }

    std::string HttpPageContent::GetContent() const
    {
        return content;
    }

    FileTimeInfo HttpPageContent::GetFileInfo() const
    {
        return fileInfo;
    }
};