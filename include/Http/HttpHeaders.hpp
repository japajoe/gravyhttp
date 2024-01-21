#ifndef GRAVY_HTTPHEADERS_HPP
#define GRAVY_HTTPHEADERS_HPP

#include <unordered_map>
#include <string>

namespace Gravy::Http
{
    using HttpHeaders = std::unordered_map<std::string,std::string>;
};

#endif