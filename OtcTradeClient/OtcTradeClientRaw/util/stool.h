#ifndef STOOL_H
#define STOOL_H
#include <string>

namespace stool
{
    std::string uniqueGroupId(const char* userId);
    std::string strToUpper(std::string str);
    std::string uniqueUserId(int broker, int type, char* user);
}

#endif // STOOL_H
