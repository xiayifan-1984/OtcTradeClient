#ifndef INNERMSGCODE_H
#define INNERMSGCODE_H
#include <QtCore>
#include <QString>
#include <string>
#include "XTTradestruct.h"
#include "XTTwapStruct.h"
#include "XTTParkedStruct.h"


inline QString parkOrderStatusToStr(char s)
{
    QString ret;
    if(s == '1')
    {
        ret = "未报";
    }
    if(s == '2')
    {
        ret = "已报";
    }
    if(s == '3')
    {
        ret = "已撤";
    }
    return ret;
}

inline QString condTriggerToStr(XTContingentConditionType trigger)
{
    QString ret = "未知";
    if(trigger == XT_CC_LastPriceGreaterThanStopPrice || trigger == XT_CC_AskPriceGreaterThanStopPrice || trigger == XT_CC_BidPriceGreaterThanStopPrice)
    {
        ret = ">";
    }
    if(trigger == XT_CC_LastPriceGreaterEqualStopPrice || trigger == XT_CC_AskPriceGreaterEqualStopPrice || trigger == XT_CC_BidPriceGreaterEqualStopPrice)
    {
        ret = ">=";
    }
    if(trigger == XT_CC_LastPriceLesserThanStopPrice || trigger == XT_CC_AskPriceLesserThanStopPrice || trigger == XT_CC_BidPriceLesserThanStopPrice)
    {
        ret = "<";
    }
    if(trigger == XT_CC_LastPriceLesserEqualStopPrice || trigger == XT_CC_AskPriceLesserEqualStopPrice|| trigger ==XT_CC_BidPriceLesserEqualStopPrice)
    {
        ret = "<=";
    }
    return  ret;
}


#pragma pack(1)

typedef struct
{
    int					broker;
    char				userid[16];
}tagTDUserInfo;


typedef struct
{
    unsigned char		mainType;				//主类型
    unsigned char		childType;				//子类型
    unsigned short		size;					//数据体的长度
}tagComm_FrameHead, MAIN_FRAME_HEAD;

#pragma pack()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Codeinnermsg
{
    std::string     otcOptInquiryReq(const QString&, int, const QString&);
}

#endif // INNERMSGCODE_H
