#ifndef STOOL_H
#define STOOL_H
#include <string>
#include <QString>

namespace stool
{
    std::string uniqueGroupId(const char* userId);
    std::string strToUpper(std::string str);
    std::string strToLower(std::string str);
    std::string uniqueUserId(int broker, int type, char* user);
    std::string genParkUserId(int broker, char* user);
    std::string loginName();
    QString     openClose2Text(char* ocflag);
    QString     buySell2Text(char bsflag);
}

#endif // STOOL_H
