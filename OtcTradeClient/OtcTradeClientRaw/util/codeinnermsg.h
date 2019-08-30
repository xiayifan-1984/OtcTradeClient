#ifndef INNERMSGCODE_H
#define INNERMSGCODE_H
#include <QtCore>
#include <QString>
#include <string>
#include <XTTradestruct.h>

/////////////////////////////////////////////////////////////////////////
///XTParkedOrderStatusType是一个预埋单/条件单状态类型
/////////////////////////////////////////////////////////////////////////
///未发送
#define XT_PAOS_NotSend     '1'
///已发送
#define XT_PAOS_Send         '2'
///已删除
#define XT_PAOS_Deleted     '3'

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

/////////////////////////////////////////////////////////////////////////
///XTContingentConditionType是一个触发条件类型
/////////////////////////////////////////////////////////////////////////
///立即
#define XT_CC_Immediately '1'
///止损
#define XT_CC_Touch '2'
///止赢
#define XT_CC_TouchProfit '3'
///预埋单
#define XT_CC_ParkedOrder '4'
///最新价大于条件价
#define XT_CC_LastPriceGreaterThanStopPrice '5'
///最新价大于等于条件价
#define XT_CC_LastPriceGreaterEqualStopPrice '6'
///最新价小于条件价
#define XT_CC_LastPriceLesserThanStopPrice '7'
///最新价小于等于条件价
#define XT_CC_LastPriceLesserEqualStopPrice '8'
///卖一价大于条件价
#define XT_CC_AskPriceGreaterThanStopPrice '9'
///卖一价大于等于条件价
#define XT_CC_AskPriceGreaterEqualStopPrice 'A'
///卖一价小于条件价
#define XT_CC_AskPriceLesserThanStopPrice 'B'
///卖一价小于等于条件价
#define XT_CC_AskPriceLesserEqualStopPrice 'C'
///买一价大于条件价
#define XT_CC_BidPriceGreaterThanStopPrice 'D'
///买一价大于等于条件价
#define XT_CC_BidPriceGreaterEqualStopPrice 'E'
///买一价小于条件价
#define XT_CC_BidPriceLesserThanStopPrice 'F'
///买一价小于等于条件价
#define XT_CC_BidPriceLesserEqualStopPrice 'H'

typedef char XTContingentConditionType;

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

struct tagXTParkedOrderField : public tagXTInputOrderField
{
    char        ParkedType; //XTContingentConditionType
    char_13     ParkedOrderID;
    char        ParkedStatus; //XTParkedOrderStatusType
    double      StopPrice;
};

typedef struct
{
    char            ParkedType; //XTContingentConditionType
    char_13         ParkedOrderID;
    char            ActionFlag;

    int             BrokerID;
    char_16         UserID;
    tagXTInstrument     ExCode;
}tagXTParkedOrderActionField;

typedef struct
{
    int             BrokerID;
    char_16         UserID;

    tagXTInstrument ExCode;
    char            ParkedType;
    char_13         ParkedOrderID;
}tagXTQryParkedOrderField;


namespace Codeinnermsg
{
    std::string otcOptInquiryReq(const QString&, int, const QString&);
}
#endif // INNERMSGCODE_H
