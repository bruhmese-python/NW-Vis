#include "itemproperties.h"

std::string itemProperties::toText(std::map<std::string, std::string> properties)
{
    std::string text;
    for(const auto &property : properties)
        text += property.first + " : " + property.second;
    return text;
}
