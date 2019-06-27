#include <string>
#include <ctime>
#include "./XTBase.h"
#include "./datamodule.h"
#include <string>
#include <algorithm>
#include "stool.h"

std::string stool::uniqueGroupId(const char* userId)
{
    std::string groupid("");
    std::time_t t = std::time(nullptr);
    groupid.append(userId).append(std::to_string(t));
    return groupid;
}

std::string stool::strToUpper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}
