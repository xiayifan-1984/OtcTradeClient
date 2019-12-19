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
#include <QTime>
#include "configmodule.h"
#include "XTCodec.h"
#include <utility>

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

std::string stool::genUserId(int broker, char *user)
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

QString   stool::hedge2Text(char hedgeFlag)
{
    QString str;
    if(hedgeFlag == XT_HF_Speculation)
        str = "投机";
    else if(hedgeFlag == XT_HF_Hedge)
        str = "套保";
    else if(hedgeFlag == XT_HF_Arbitrage)
        str = "套利";
    else
        str = "其他";
    return str;
}

bool stool::isMarketAlive(const string& inst, const QTime &begin, const QTime &end)
{
    QTime b = begin;
    QTime e = end;

    auto pConf = GetConfigModule();
    if(pConf)
    {
        auto& ts = pConf->GetMarketOpenedTimeSec();
        auto search = ts.find(inst);
        if(search != ts.end())
        {
            auto search_begin = find_if(search->second.begin(), search->second.end(), [&](const pair<QTime, QTime>& t){
                   return b >= t.first && b <= t.second;
            });
            auto search_end = find_if(search->second.begin(), search->second.end(), [&](const pair<QTime, QTime>& t){
                    return e >= t.first && e <= t.second;
             });
            return (search_begin != search->second.end() && search_end != search->second.end()) ? true:false;
        }
        return false;
    }
    return false;
}

bool stool::isOrderStatusCauseUpdate(XTOrderStatusType curStatus, XTOrderStatusType newStatus)
{
    if(curStatus == newStatus) return false;
    if(curStatus == XT_OST_Accepted || curStatus == XT_OST_Canceled) return false;
    return true;
}
