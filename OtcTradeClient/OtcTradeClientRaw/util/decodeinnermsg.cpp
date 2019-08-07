#include "decodeinnermsg.h"
#include <QtCore>
#include <QDebug>
#include "string.h"
#include "XTCodec.h"
#include <string>

bool Decodeinnermsg::decodeOtcOptPositionRsp(const char *buf, int len, otcOptPositionRsp& allOtcOptInfo)
{
    if(!buf || len == 0) return false;
    std::string tmpdata(buf);
    QString qs = XTCodec::AfUtf8_ToQString(tmpdata);
    QByteArray data = qs.toLatin1();
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(data, &jsonError);

    if(document.isNull() or jsonError.error != QJsonParseError::NoError)
    {
        return false;
    }
    if(!document.isObject()) return false;

    QJsonObject rootobj = document.object();
    if(rootobj.contains("resID"))
    {
        QJsonValue value = rootobj.value("resID");
        allOtcOptInfo.resID = value.toString().toStdString();
    }
    if(rootobj.contains("requestID"))
    {
        QJsonValue value = rootobj.value("requestID");
        auto num = value.toInt();
        allOtcOptInfo.reqID = num;
    }

    if(rootobj.contains("rows"))
    {
        QJsonValue value = rootobj.value("rows");
        if(value.isArray())
        {
            QJsonArray jarr = value.toArray();
           // allOtcOptInfo.positions.resize(jarr.size());
            auto p = jarr.begin();
           // auto q = allOtcOptInfo.positions.begin();
            while(p != jarr.end())
            {
                otcOptPosition result;
                memset(&result, 0, sizeof(result));
                if(p->isObject())
                {
                    const auto& subobj = p->toObject();
                    if(subobj.contains("status"))
                    {
                        auto val = subobj.value("status");
                        result.status = val.toInt();
                    }
                    if(subobj.contains("trade_no"))
                    {
                        auto val = subobj.value("trade_no");
                        result.trade_no = val.toString().toStdString();
                    }
                    if(subobj.contains("user_name"))
                    {
                        auto val = subobj.value("user_name");
                        result.user_name = val.toString().toStdString();
                    }
                    if(subobj.contains("user_no"))
                    {
                        auto val = subobj.value("user_no");
                        result.user_no = val.toString().toStdString();
                    }
                    if(subobj.contains("direction"))
                    {
                        auto val = subobj.value("direction");
                        auto num = val.toInt();
                        result.direction = num;
                    }
                    if(subobj.contains("volume"))
                    {
                        auto val = subobj.value("volume");
                        auto num = val.toDouble();
                        result.volume = num;
                    }
                    if(subobj.contains("under_product"))
                    {
                        auto val = subobj.value("under_product");
                        result.under_product = val.toString().toStdString();
                    }
                    if(subobj.contains("under_code"))
                    {
                        auto val = subobj.value("under_code");
                        result.under_code = val.toString().toStdString();
                    }
                    if(subobj.contains("trade_date"))
                    {
                        auto val = subobj.value("trade_date");
                        auto num = val.toInt();
                        result.trade_date = num;
                    }
                    if(subobj.contains("exec_type"))
                    {
                        auto val = subobj.value("exec_type");
                        auto num = val.toInt();
                        result.exec_type = num;
                    }
                    if(subobj.contains("option_struct"))
                    {
                        auto val = subobj.value("option_struct");
                        auto num = val.toInt();
                        result.option_struct = num;
                    }
                    if(subobj.contains("exec_date"))
                    {
                        auto val = subobj.value("exec_date");
                        auto num = val.toInt();
                        result.exec_date = num;
                    }
                    if(subobj.contains("exec_price"))
                    {
                        auto val = subobj.value("exec_price");
                        auto num = val.toDouble();
                        result.exec_price = num;
                    }
                    if(subobj.contains("callput"))
                    {
                        auto val = subobj.value("callput");
                        auto num = val.toInt();
                        result.callput = num;
                    }
                    if(subobj.contains("enddate"))
                    {
                        auto val = subobj.value("enddate");
                        auto num = val.toInt();
                        result.enddate = num;
                    }
                    if(subobj.contains("price"))
                    {
                        auto val = subobj.value("price");
                        auto num = val.toDouble();
                        result.price = num;
                    }
                    if(subobj.contains("under_price"))
                    {
                        auto val = subobj.value("under_price");
                        auto num = val.toDouble();
                        result.under_price = num;
                    }
                    if(subobj.contains("multi"))
                    {
                        auto val = subobj.value("multi");
                        auto num = val.toInt();
                        result.multi = num;
                    }
                    if(subobj.contains("trader"))
                    {
                        auto val = subobj.value("trader");
                        result.trader = val.toString().toStdString();
                    }
                    if(subobj.contains("under_sigma"))
                    {
                        auto val = subobj.value("under_sigma");
                        auto num = val.toDouble();
                        result.under_sigma = num;
                    }
                }
                ++p;
                allOtcOptInfo.positions.push_back(result);
            }
        }
    }
    return true;
}

otcOptPosition::otcOptPosition()
{
    status = 0;
    trade_no = "";
    user_name = "";
    user_no = "";
    direction = -1;
    volume = 0;
    under_product = "";
    under_code = "";
    trade_date = 0;
    exec_type = 0;
    option_struct = 0;
    exec_date = 0;
    exec_price = 0.0;
    callput = -1;
    enddate = 0;
    price = 0.0;
    under_price = 0.0;
    multi = 0.1;
    trader = "";
    under_sigma = 0.01;
}

otcOptPositionRsp::otcOptPositionRsp()
{
    resID = "";
    reqID = 0;
}
