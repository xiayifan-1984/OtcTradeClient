#ifndef ORDERMGRIMPL_DEFINE_H
#define ORDERMGRIMPL_DEFINE_H

#include "trademodule.h"
#include "XTTradeApi.h"
#include <windows.h>
#include <string>
#include <vector>
#include "./util/decodeinnermsg.h"
using namespace std;

class QOrderMgrImpl:public QOrderMgr, public CXTTradeSpi
{
public:
    QOrderMgrImpl(HMODULE hDll, CXTTradeApi* pApi);
    int             Instance(HWND hWnd, unsigned int nMsg, int broker, int usertype, char* pszuser);
    void            Release();
    void            setHWnd(HWND hWnd, unsigned int nMsg);

public:
    void            WhoIs(int& type, int& broker,  char*  userid  );

//QOrderMgr
public:
    int			GetAllOrder(tagXTOrderField* pArr, int nCount);
    int			GetAllTrade( tagXTTradeField* pArr, int nCount);
    int			GetAllPosition( tagXTInvestorPositionField* pArr, int nCount);

    int			InsertOrder(tagXTInputOrderField* pIn);
    int			CancelOrder(tagXTInputOrderActionField* pIn);

    int			GetOneOrder(const char* pszOrderRef, tagXTOrderField& oOut);
    int			GetOnePosition(tagXTInstrument& oExCode,char LongShort, char  HedgeFlag, tagXTInvestorPositionField& oOut);
    int			GetTradingAccount(tagXTTradingAccountField* pIn);
    void        GetOrdersForInstrument(tagXTInstrument&, std::vector<tagXTOrderField>&);
    void        GetPositionsForInstrument(tagXTInstrument&, std::vector<tagXTInvestorPositionField>&);

//CXTTradeSpi
public:
    void        OnFrontConnected();
    void        OnFrontDisconnected(int nReason);

    void        OnRspUserLogin(tagXTRspInfoField *pRspInfo, bool bIsLast);

    void        OnRspQryOrder(tagXTOrderField *pOrder, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast);
    void        OnRspQryTrade(tagXTTradeField *pTrade, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast);
    void        OnRspQryInvestorPosition(tagXTInvestorPositionField *pInvestorPosition, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast);
    void        OnRspQryTradingAccount(tagXTTradingAccountField* pAccount, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast);

    void        OnRspOrderInsert(tagXTInputOrderField *pInputOrder, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast);

    void        OnRtnOrder(tagXTOrderField *pOrder);
    void        OnRtnTrade(tagXTTradeField *pTrade);

protected:
    void        SendNotify(WPARAM w, LPARAM l);
    void        VerifyExCode(tagXTInstrument& inout);

    string      inner_getkey(unsigned char e, const char* pcode, char offset, char  hedge);
    //更新持仓
    void        inner_updateposition(tagXTTradeField *pTrade, bool bpush);
    void		inner_updateposition_future(tagXTTradeField* pTrade, bool bpush);
    void        inner_reArrange();

protected:
    HWND				m_hWnd;
    unsigned int		m_nMsg;
    int                 m_nbroker;
    int                 m_nusertype;
    char                m_szuser[32];

protected:
    HMODULE             m_hdll;
    CXTTradeApi*        m_pTradeApi;
    bool				m_bInitOk;
    int					m_nRequestID;

protected:
    tagXTTradingAccountField					m_oTradingAccount;		//
    map<string, tagXTTradeField>				m_mapTrade;				//tradeid  ===> trade
    map<string, tagXTInvestorPositionField>     m_mapPosition;			//ex_code_longshort_hedgeflag ===> position
    map<int,	tagXTOrderField>				m_mapBrachOrder;
    map<string, int>                            m_mapOrder;				//orderref  ===> brokerseq
    map<string, otcOptPosition>                 m_mapOtcOptPositions;
};



















#endif
