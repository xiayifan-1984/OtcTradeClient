#ifndef CONFIGMODULEDEFINE_H
#define CONFIGMODULEDEFINE_H

#include "XTStaticData.h"
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <set>
#include <QTime>
using namespace std;

//================================================================================================================================================================================================================================================================================
#define    RUNMODE_NORMAL       0       //正常模式，有数据，有行情，有交易
#define    RUNMODE_ONLYDATA     1       //仅数据
#define    RUNMODE_DATA2QUOTE   2       //有数据，有行情



#pragma pack(1)

typedef struct
{
    int         RunMode;                //RUNMODE_
   
    char        ExeDir[255];            //EXE所在目录
    

    char        ShareDataDir[255];      //共享数据目录
    char        UserDir[255];           //当前用户目录
    char        CurUser[255];
}tagTGlobalParam;

typedef struct
{
    char        TradeLine[255];         //TradeLine.ini的路径
    char        TradeUser[255];         //TradeUser.ini的路径
    int         TradeUserCount;         //交易用户的个数

    double      vol;                    //Volatility
    double      riskFreeRate;           // risk free rate , default 4.5%
    char        kafkaServer[255];
}tagTSystemParam;



typedef struct
{
    int         broker;             //broker
    int         type;               //type
    char        user[32];           //user
    char        aliasuser[64];      //别名
    char        tradelimit[512];    //交易范围
}tagTTradeUserParam;

typedef struct
{
    int         reqInsertNo;        //上一次下单的序号
    int         lastReqTime;        //上一次下单的 时间
    int         reqCancelNo;        //上一次撤单的序号
    int         lastCancelTime;     //上一次撤单的时间

    //......

}tagTDynamicMemory;

#pragma pack()

typedef  unordered_map<string, vector<pair<QTime, QTime>>> TimeSections;
//================================================================================================================================================================================================================================================================================
class QConfigModule
{
public:
    QConfigModule();

    int                         Init();
    void                        Free();
    void                        ReloadTradeUser();

public:
    //[1]
    int                         GetTradeUserBy(const char* pProductID, vector<tagTTradeUserParam>& arrUser);
    int                         GetAllTradeUser(vector<tagTTradeUserParam>& arrUser);
    //[3]
    void                        GetOrderRef(char* pszOutOrderRef,  char* pszMarker);

    void                        InsertRiskVol(tagXTInstrument& ExCode, double vol);
    double                      GetRiskVol(tagXTInstrument& code);
    const TimeSections&         GetMarketOpenedTimeSec();

public:
    tagTGlobalParam             g;
    tagTSystemParam             s;
    tagTDynamicMemory           m;

protected:
    void                        initTradeUser();
    void                        initClient();
    void                        initTradeTimeSection();
    void                        initVolatilitys();
    void                        storeVolatilitys();

protected:
    vector<tagTTradeUserParam>     m_arrTradeUser;

    unordered_map<string, vector<pair<QTime, QTime>>> m_tradeTimeSection;
    unordered_map<string, double>   m_mapRiskVols;
};


extern QConfigModule*      GetConfigModule();



#endif
