#ifndef TRADEMODULEDEFINE_H
#define TRADEMODULEDEFINE_H

#include <QtCore>
#include "XTTradestruct.h"
#include <windows.h>
#include <vector>

//=============================================================================================================================================================================================================================================================================================================
class QOrderMgr
{
public:
    virtual ~QOrderMgr();
protected:
    QOrderMgr();

public:
    virtual		int			GetAllOrder(tagXTOrderField* pArr, int nCount) = 0;
    virtual		int			GetAllTrade( tagXTTradeField* pArr, int nCount) = 0;
    virtual		int			GetAllPosition( tagXTInvestorPositionField* pArr, int nCount) = 0;

    virtual		int			InsertOrder(tagXTInputOrderField* pIn) = 0;
    virtual		int			CancelOrder(tagXTInputOrderActionField* pIn) = 0;
    //virtual		int			ManualReqAccount() = 0;

public:
    virtual		int			GetOneOrder(const char* pszOrderRef, tagXTOrderField& oOut) =0;
    virtual		int			GetOnePosition(tagXTInstrument& oExCode,char LongShort, char  HedgeFlag, tagXTInvestorPositionField& oOut) =0;
    virtual		int			GetTradingAccount(tagXTTradingAccountField* pIn) =0;
    virtual     void        GetOrdersForInstrument(tagXTInstrument&, std::vector<tagXTOrderField>&) = 0;
    virtual     void        GetPositionsForInstrument(tagXTInstrument&, std::vector<tagXTInvestorPositionField>&) = 0;

public:
    virtual      void       WhoIs(int& type, int& broker,  char*  userid  ) =0;

};


class QTradeModule
{
public:
    virtual ~QTradeModule();
protected:
    QTradeModule();

public:
    virtual     int         Init() = 0;
    virtual     void        Free() = 0;

    virtual     int         Reload(const char* plinefile, const char* puserfile) = 0;
    virtual     int         StartWork(HWND hWnd, unsigned int nMsg) = 0;
    virtual     void        EndWork() = 0;

    virtual     QOrderMgr*  GetMgr(int type, int broker, const char* pszuser) = 0;
    virtual     void        SetHWnd(HWND hWnd, unsigned int nMsg) = 0;

//signal

};


extern  QTradeModule*        GetTradeModule();





#endif

