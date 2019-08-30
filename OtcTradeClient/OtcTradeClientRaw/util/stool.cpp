#include <string>
#include <ctime>
#include "./XTBase.h"
#include "./datamodule.h"
#include <string>
#include <algorithm>
#include "stool.h"
#include "configmodule.h"
#include <QString>
#include "XTTradestruct.h"

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


std::string stool::loginName()
{

    auto pConf = GetConfigModule();
    if(pConf)
    {
        std::string result(pConf->g.CurUser);
        return result;
    }
    return "";
}

string stool::strToLower(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}



QString stool::openClose2Text(char *ocflag)
{
    QString str;
    if (ocflag[0] == XT_OF_Open)
        str = "开仓";
    else if (ocflag[0] == XT_OF_Close)
        str = "平仓";
    else if (ocflag[0] == XT_OF_CloseToday)
        str = "平今";
    else if (ocflag[0] == XT_OF_CloseYesterday)
        str = "平昨";
    else if (ocflag[0] == XT_OF_ForceClose)
        str = "强平";
    else if (ocflag[0] == XT_OF_Auto)
        str = "自动";
    else
        str = "其他";
    return str;
}

QString stool::buySell2Text(char bsflag)
{
    QString str;
    if(bsflag == XT_D_Buy)
        str = "买";
    else if(bsflag == XT_D_Sell)
        str = "卖";
    else
        str = "其他";
    return str;
}
