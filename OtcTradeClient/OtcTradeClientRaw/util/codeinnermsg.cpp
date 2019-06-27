#include "codeinnermsg.h"
#include "XTCodec.h"

std::string Codeinnermsg::otcOptInquiryReq(const QString &resId, int reqId, const QString &clientId)
{
    QVariantHash data;
    data.insert("resID", resId);
    data.insert("requestID", reqId);
    data.insert("clientID", clientId);

    QJsonObject rootObj = QJsonObject::fromVariantHash(data);
    QJsonDocument document;
    document.setObject(rootObj);
    QString json_str(document.toJson(QJsonDocument::Compact));
    auto result =  XTCodec::AfUtf8_ToString(json_str);
    return result;
}

