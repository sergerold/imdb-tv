#ifndef CURLANDCLEAN_H
#define CURLANDCLEAN_H

#include <iostream>
#include "../pugixml/src/pugixml.hpp"

std::string getCleanedHTML (std::string URL);
std::string removecommas (std::string);
std::string removenewlines(std::string unformattedstr);
std::string trim_whitespace (std::string);
std::string node_to_string(pugi::xml_node node);
std::string removeStr(std::string str, std::string toRemove);

const std::string baseURL = "http://www.imdb.com";

#endif /* CURLANDCLEAN_H */
