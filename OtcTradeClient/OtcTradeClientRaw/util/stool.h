#ifndef STOOL_H
#define STOOL_H
#include <string>
#include <QString>
#include <QTime>
#include "XTTradestruct.h"

namespace stool
{
    std::string uniqueGroupId(const char* userId);
    std::string strToUpper(std::string str);
    std::string strToLower(std::string str);
    std::string uniqueUserId(int broker, int type, char* user);
    std::string genUserId(int broker, char* user);
    std::string loginName();
    QString     openClose2Text(char* ocflag);
    QString     buySell2Text(char bsflag);
    bool        isMarketAlive(const std::string & inst, const QTime& begin, const QTime& end);
    bool        isOrderStatusCauseUpdate(XTOrderStatusType curStatus, XTOrderStatusType newStatus);
}
#endif // STOOL_H
