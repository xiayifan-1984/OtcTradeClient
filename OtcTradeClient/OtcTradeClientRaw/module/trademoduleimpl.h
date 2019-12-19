#ifndef TRADEMODULEIMPL_DEFINE_H
#define TRADEMODULEIMPL_DEFINE_H

#include "trademodule.h"
#include "XTTradeApi.h"
#include <vector>
#include <string>
#include <map>
#include <queue>
using namespace std;

#include <windows.h>

class QOrderMgrImpl;

class QTradeModuleImpl : public QTradeModule
{
public:
    QTradeModuleImpl();

public:
    int         Init();
    void        Free();

    int         Reload(const char* plinefile, const char* puserfile);
    int         StartWork(HWND hWnd, unsigned int nMsg);
    void        EndWork();
    void        SetHWnd(HWND hWnd, unsigned int nMsg);

    QOrderMgr*  GetMgr(int type, int broker, const char* pszuser);

protected:
    void        loadLineFile(const char* plinefile);
    void        loadUserFile(const char* puserfile);
    void        initDllLoader();
    void        initOneUser();
    HMODULE     findDllFromLine(int lineno);

protected:
    struct tagTradeLine
    {
        int         id;
        char        dll[255];

        char        front[255];
        char        tradedll[255];
        bool        needauth;
    };
    std::vector<tagTradeLine>       m_arrLines;

    struct  tagTradeUser
    {
        int         type;
        int         broker;
        char        user[64];
        char        psw[64];
        char        userproductinfo[64];
        char        authcode[64];
        int         lineno;
    };
    vector<tagTradeUser>       m_arrUsers;

    map<string, QOrderMgrImpl*>   m_mapUser;      //string=broker_utype_user, QOrderMgrImpl
    map<int, HMODULE>          m_mapDll;       //lineno ===>dll

protected:
    HWND            m_hWnd;
    unsigned int    m_nMsg;

public:
    vector<tagTradeUser>& getAllUsers()
    {
        return m_arrUsers;
    }

};



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
    int         ReqQryAccount();

    int			GetTradingAccount(tagXTTradingAccountField* pIn);
    const map<string, string>& getErrorMsgsInfo() const;
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

    string      inner_getkey(unsigned char e, const char* pcode, char offset, char  hedge, int posdate = 0);   //持仓的Key
    //更新持仓
    void        inner_updateposition(tagXTTradeField *pTrade, bool bpush);
    void		inner_updateposition_future(tagXTTradeField* pTrade, bool bpush);
    void        inner_reArrange();

protected:
    int         inner_existorder(tagXTOrderField* pOrder);      //查找Order是否已被记录，如果已被记录，返回 位置，否则返回-1
    void        inner_addneworder(tagXTOrderField* pOrder);     //增加Order, 增加map中的索引
    void        inner_doNextTask();

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

    map<string, tagXTInvestorPositionField>     m_mapOriPosition;       //ex_code_longshrot_hedgeflag_posdate ===> position  //原始Position列表
    map<string, tagXTInvestorPositionField>     m_mapPosition;			//ex_code_longshort_hedgeflag ===> position

    vector<tagXTOrderField>                     m_arrBranchOrder;       //所有委托单
    map<string, int>                            m_mapSysid2Int;         //SysID ===>  vector中的序号
    map<string, int>                            m_mapRef2Int;           //Front_Session_OrdeRef ===> vector 中的序号

protected:
    map<string, string>                         m_errorOrderMsg;        //orderRef ===>  ErrorMsg
    queue<int>                                  m_queueTask;            //任务队列，

};




#endif
