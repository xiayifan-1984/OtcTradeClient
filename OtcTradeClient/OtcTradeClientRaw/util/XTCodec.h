#ifndef XTCODECDEFINE_H
#define XTCODECDEFINE_H
#include <string>
#include <QString>

namespace XTCodec
{
    // QString(Unicode) -> std::string (UTF8)
    std::string     AfUtf8_ToString(const QString& qstr);

    // std::string (UTF8) -> QString(Unicode)
    QString         AfUtf8_ToQString(const std::string& cstr);

    // QString(Unicode) -> std::string (GBK)
    std::string     AfGbk_ToString(const QString& qstr);

    // std::string (GBK) -> QString(Unicode)
    QString         AfGbk_ToQString(const std::string& cstr);
};


#endif
