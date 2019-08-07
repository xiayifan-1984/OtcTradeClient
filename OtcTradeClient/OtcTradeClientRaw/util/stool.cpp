#include <string>
#include <ctime>
#include "./XTBase.h"
#include "./datamodule.h"
#include <string>
#include <algorithm>
#include "stool.h"
#include "configmodule.h"

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

std::string stool::uniqueUserId(int broker, int type, char *user)
{
    std::string result(user);
    result += std::to_string(broker);
    result += std::to_string(type);
    return result;
}

std::string stool::genParkUserId(int broker, char *user)
{

    if(!user) return  "";
    char key[255]{0};
    sprintf(key, "%d_%s", broker, user);
    std::string result(key);
    return key;
}

string stool::loginName()
{
    auto pConf = GetConfigModule();
    if(pConf)
    {
        std::string result(pConf->g.CurUser);
        return result;
    }
    return "";
}
