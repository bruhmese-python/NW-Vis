#ifndef ITEMPROPERTIES_H
#define ITEMPROPERTIES_H

#include <map>
#include <string>

using PROPERTIES = std::map<std::string,std::string>;

namespace itemProperties
{
    std::string toText(PROPERTIES);
};

#endif // ITEMPROPERTIES_H
