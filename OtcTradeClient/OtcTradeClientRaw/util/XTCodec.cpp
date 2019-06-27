
#include "XTCodec.h"
#include <QTextCodec>

// QString(Unicode) -> std::string (UTF8)
std::string     XTCodec::AfUtf8_ToString(const QString& qstr)
{
    QByteArray arr = qstr.toUtf8();
    std::string cstr = arr.data();
    return cstr;
}

// std::string (UTF8) -> QString(Unicode)
QString         XTCodec::AfUtf8_ToQString(const std::string& cstr)
{
    QString qstr = QString::fromUtf8(cstr.c_str(), cstr.length());
    return qstr;
}

// QString(Unicode) -> std::string (GBK)
std::string     XTCodec::AfGbk_ToString(const QString& qstr)
{
    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
    if(!pCodec) return "";

    QByteArray arr = pCodec->fromUnicode(qstr);
    std::string cstr = arr.data();
    return cstr;
}

// std::string (GBK) -> QString(Unicode)
QString         XTCodec::AfGbk_ToQString(const std::string& cstr)
{
    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
    if(!pCodec) return "";

    QString qstr = pCodec->toUnicode(cstr.c_str(), cstr.length());
    return qstr;
}
