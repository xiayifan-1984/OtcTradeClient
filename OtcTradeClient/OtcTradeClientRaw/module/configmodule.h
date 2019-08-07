#ifndef CONFIGMODULEDEFINE_H
#define CONFIGMODULEDEFINE_H

#include "XTStaticData.h"
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <set>
using namespace std;

//================================================================================================================================================================================================================================================================================
#define    RUNMODE_NORMAL       0       //正常模式，有数据，有行情，有交易
#define    RUNMODE_ONLYDATA     1       //仅数据
#define    RUNMODE_DATA2QUOTE   2       //有数据，有行情



#pragma pack(1)

typedef struct
{
    int         RunMode;                //RUNMODE_
    double      vol;                    //Volatility
    double      riskFreeRate;           // risk free rate , default 4.5%
    char        ExeDir[255];            //EXE所在目录
    char        kafkaServer[255];

    char        ShareDataDir[255];      //共享数据目录
    char        UserDir[255];           //当前用户目录
    char        CurUser[255];
}tagGlobalSetting;

typedef struct
{
    char        TradeLine[255];         //TradeLine.ini的路径
    char        TradeUser[255];         //TradeUser.ini的路径

    int         TradeUserCount;         //交易用户的个数
}tagSessionSetting;

typedef struct
{
    int         broker;             //broker
    int         type;               //type
    char        user[32];           //user
    char        aliasuser[64];      //别名
    char        tradelimit[512];    //交易范围
}tagOneTradeUser;

typedef struct
{
    int         reqInsertNo;        //上一次下单的序号
    int         lastReqTime;        //上一次下单的 时间
    int         reqCancelNo;        //上一次撤单的序号
    int         lastCancelTime;     //上一次撤单的时间

    //......

}tagMemorySave;

#pragma pack()


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
    int                         GetTradeUserBy(const char* pProductID, vector<tagOneTradeUser>& arrUser);
    int                         GetAllTradeUser(vector<tagOneTradeUser>& arrUser);
    //[2]
    int                         GetAllMainContract(vector<tagXTInstrument>& arrExCode);
    //[3]
    void                        GetOrderRef(char* pszOrderRef,  char* pszMarker);
    void                        insertRiskVol(tagXTInstrument& code, double vol);
    double                      getRiskVol(tagXTInstrument& code);



public:
    tagGlobalSetting            g;
    tagSessionSetting           s;
    tagMemorySave               m;

protected:
    void                        initTradeUser();
    void                        initClient();
    void                        initMainContract();
    void                        initTradeTimeSection();
    void                        initVolatilitys();
    void                        storeVolatilitys();

protected:
    vector<tagOneTradeUser>     m_arrTradeUser;
    unordered_map<string, vector<string>> m_tradableProducts;
    vector<tagXTInstrument>     m_arrMainContract;
    unordered_map<string, vector<pair<string, string>>> m_tradeTimeSection;
    unordered_map<string, double> m_instRiskVols;
};


extern QConfigModule*      GetConfigModule();



#endif
