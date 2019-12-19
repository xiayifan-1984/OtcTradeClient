
#include "trademoduleimpl.h"
#include "datamodule.h"
#include "eventcentermodule.h"
#include "XTCodec.h"
#include "configmodule.h"
#include "stool.h"

//=================================================================================================================================================================================================================================================================================================
//=================================================================================================================================================================================================================================================================================================
//=================================================================================================================================================================================================================================================================================================
//=================================================================================================================================================================================================================================================================================================

#pragma pack(1)

typedef struct
{
    char			tradedll[255];
}tagXTTradeDll;

typedef struct
{
    bool			needauth;
    char			frontaddr[255];
}tagXTLineSetting;

typedef struct
{
    int				userType;
    int				broker;
    char			user[64];
    char			psw[64];
    char			userproductinfo[64];
    char			authcode[64];
}tagXTUserSetting;

typedef struct
{
    tagXTLineSetting		line;
    tagXTUserSetting		user;
}tagXTCreateTradeParam;

#pragma pack()

//=================================================================================================================================================================================================================================================================================================
//=================================================================================================================================================================================================================================================================================================
//=================================================================================================================================================================================================================================================================================================
//=================================================================================================================================================================================================================================================================================================

QTradeModuleImpl::QTradeModuleImpl()
{
    m_hWnd =0;
    m_nMsg =0;
}

int         QTradeModuleImpl::Init()
{
    m_mapUser.clear();
    m_mapDll.clear();
    m_arrLines.clear();
    m_arrUsers.clear();

    return 1;
}

void        QTradeModuleImpl::Free()
{
    m_arrLines.clear();
    m_arrUsers.clear();
}

int         QTradeModuleImpl::Reload(const char* plinefile, const char* puserfile)
{
    m_arrLines.clear();
    m_arrUsers.clear();

    char szinifile[255]={0};
    strcpy(szinifile, plinefile);
    loadLineFile(szinifile);

    memset(szinifile, 0, sizeof(szinifile));
    strcpy(szinifile, puserfile);
    loadUserFile(szinifile);

    m_mapUser.clear();
    m_mapDll.clear();

    return 1;
}

QOrderMgr*  QTradeModuleImpl::GetMgr(int type, int broker, const char* pszuser)
{
    char szbuf[255]={0};
    _snprintf(szbuf, 254, "%d_%d_%s", broker, type, pszuser);

    string strkey = szbuf;
    map<string, QOrderMgrImpl*>::iterator itb = m_mapUser.find(strkey);
    if (itb != m_mapUser.end())
    {
        return itb->second;
    }

    return 0;
}

void        QTradeModuleImpl::loadLineFile(const char* plinefile)
{
    int ncount = GetPrivateProfileIntA("main", "linecount", 0, plinefile);
    for(int i=0; i< ncount; i++)
    {
        tagTradeLine oinfo;
        memset(&oinfo, 0, sizeof(oinfo));

        char szkey[32]={0};
        _snprintf(szkey, 31, "line_%d", i);
        oinfo.id = GetPrivateProfileIntA(szkey, "id", 0, plinefile);

        GetPrivateProfileStringA(szkey, "dll", "", oinfo.dll, 254, plinefile);

        GetPrivateProfileStringA(szkey, "addr", "", oinfo.front, 254, plinefile);
        GetPrivateProfileStringA(szkey, "trade_dll","", oinfo.tradedll, 254, plinefile);

        char szbuf[32]={0};
        GetPrivateProfileStringA(szkey, "needauth", "", szbuf, 31, plinefile);
        if(strnicmp(szbuf, "true", 4) == 0)
        {
            oinfo.needauth = true;
        }
        else
        {
            oinfo.needauth = false;
        }

        m_arrLines.push_back(oinfo);
    }
}

void        QTradeModuleImpl::loadUserFile(const char* puserfile)
{
    int ncount = GetPrivateProfileIntA("main", "usercount", 0, puserfile);
    for(int i=0; i< ncount; i++)
    {
        tagTradeUser oinfo;
        memset(&oinfo, 0, sizeof(oinfo));

        char szkey[32]={0};
        _snprintf(szkey, 31, "user_%d", i);
        oinfo.type = GetPrivateProfileIntA(szkey, "type", 0, puserfile);
        oinfo.broker = GetPrivateProfileIntA(szkey, "broker", 0, puserfile);
        GetPrivateProfileStringA(szkey, "user", "", oinfo.user, 32, puserfile);
        GetPrivateProfileStringA(szkey, "psw", "", oinfo.psw, 32, puserfile);
        GetPrivateProfileStringA(szkey, "userproductinfo", "", oinfo.userproductinfo, 32, puserfile);
        GetPrivateProfileStringA(szkey, "authcode", "", oinfo.authcode, 32, puserfile);

        oinfo.lineno = GetPrivateProfileIntA(szkey, "lineno", 0, puserfile);

        m_arrUsers.push_back(oinfo);
    }

}

void        QTradeModuleImpl::initDllLoader()
{
    int ncount = m_arrLines.size();
    for(int i=0; i<ncount; i++)
    {
        int lineno = m_arrLines[i].id;

        HMODULE hDll = LoadLibraryA(m_arrLines[i].dll);
        if(0 == hDll)
        {
            continue;
        }

        typedef int (__stdcall *pfnBeginWork)(tagXTTradeDll* p);
        pfnBeginWork pfn = (pfnBeginWork)::GetProcAddress(hDll, "BeginWork");
        if(pfn)
        {
            tagXTTradeDll oinfo;
            memset(&oinfo, 0, sizeof(oinfo));
            strcpy(oinfo.tradedll, m_arrLines[i].tradedll);
            pfn(&oinfo);
        }
        else
        {
            continue;
        }
        m_mapDll.insert( make_pair( lineno, hDll) );
    }

}

void        QTradeModuleImpl::initOneUser()
{
    int ncount = m_arrUsers.size();
    for(int i=0; i<ncount; i++)
    {
        HMODULE hDll = findDllFromLine(m_arrUsers[i].lineno);
        if(0 == hDll)
        {
            continue;
        }

        tagTradeLine* pLine = nullptr;
        for(int j=0; j < m_arrLines.size(); j++)
        {
            if(m_arrLines[j].id == m_arrUsers[i].lineno)
            {
                pLine = &(m_arrLines[j]);
                break;
            }
        }
        if(nullptr == pLine)
        {
            continue;
        }

        typedef CXTTradeApi*	(__stdcall *pfnCreateTradeApi)(tagXTCreateTradeParam* , char* );
        pfnCreateTradeApi pfn = (pfnCreateTradeApi)::GetProcAddress(hDll, "CreateTradeApi");
        if(pfn)
        {
            tagXTCreateTradeParam oParam;
            memset(&oParam, 0, sizeof(oParam));
            oParam.user.userType = m_arrUsers[i].type;
            oParam.user.broker = m_arrUsers[i].broker;
            strcpy(oParam.user.user, m_arrUsers[i].user);
            strcpy(oParam.user.psw, m_arrUsers[i].psw);
            strcpy(oParam.user.userproductinfo, m_arrUsers[i].userproductinfo);
            strcpy(oParam.user.authcode, m_arrUsers[i].authcode);

            strcpy(oParam.line.frontaddr, pLine->front);
            oParam.line.needauth = pLine->needauth;

            char szerr[256]={0};
            CXTTradeApi* pApi = pfn(&oParam, szerr);

            QOrderMgrImpl* pMgr = new QOrderMgrImpl(hDll , pApi);
            pMgr->Instance(m_hWnd, m_nMsg, m_arrUsers[i].broker, m_arrUsers[i].type, m_arrUsers[i].user);

            char szkey[128]={0};
            _snprintf(szkey, 127, "%d_%d_%s", m_arrUsers[i].broker, m_arrUsers[i].type, m_arrUsers[i].user );
            std::string strkey = szkey;
            m_mapUser.insert(make_pair( strkey  ,pMgr) );
        }

    }

}

void        QTradeModuleImpl::SetHWnd(HWND hWnd, unsigned int nMsg)
{
    m_hWnd = hWnd;
    m_nMsg = nMsg;

    map<string, QOrderMgrImpl*>::iterator itb = m_mapUser.begin();
    while(itb != m_mapUser.end() )
    {
        itb->second->setHWnd(m_hWnd, m_nMsg);
        itb++;
    }
}

int         QTradeModuleImpl::StartWork(HWND hWnd, unsigned int nMsg)
{
    m_hWnd = hWnd;
    m_nMsg = nMsg;

    initDllLoader();
    initOneUser();

    return 1;
}

void        QTradeModuleImpl::EndWork()
{
    map<string, QOrderMgrImpl*>::iterator itb = m_mapUser.begin();
    while (itb != m_mapUser.end() )
    {
        QOrderMgrImpl* pImpl = itb->second;
        pImpl->Release();

        delete pImpl;

        itb++;
    }

    m_mapUser.clear();

    map<int, HMODULE>::iterator it = m_mapDll.begin();
    while(it != m_mapDll.end() )
    {
        HMODULE hDll  = it->second;
        if(hDll)
        {
            typedef void    (__stdcall *pfnEndWork)();
            pfnEndWork pfn = (pfnEndWork)::GetProcAddress(hDll, "EndWork");
            if(pfn)
            {
                pfn();
            }

            FreeLibrary(hDll);
        }

        it++;
    }
    m_mapDll.clear();

}

HMODULE     QTradeModuleImpl::findDllFromLine(int lineno)
{
    map<int, HMODULE>::iterator it = m_mapDll.find(lineno);
    if(it != m_mapDll.end() )
    {
        return it->second;
    }

    return 0;
}

//=================================================================================================================================================================================================================================================================================================
//=================================================================================================================================================================================================================================================================================================
//=================================================================================================================================================================================================================================================================================================
//=================================================================================================================================================================================================================================================================================================
#define TASKID_QRY_ACCOUNT      1
#define TASKID_QRY_POSITION     2
#define TASKID_QRY_TRADE        3
#define TASKID_QRY_ORDER        4

QOrderMgrImpl::QOrderMgrImpl(HMODULE hDll, CXTTradeApi* pApi)
{
    m_hdll = hDll;
    m_pTradeApi = pApi;

    m_bInitOk = false;

    m_nRequestID =1;

    memset(&m_oTradingAccount, 0, sizeof(m_oTradingAccount));

    m_nbroker =0;
    m_nusertype =0;
    memset(m_szuser, 0, sizeof(m_szuser));
}

int             QOrderMgrImpl::Instance(HWND hWnd, unsigned int nMsg, int broker, int usertype, char* pszuser)
{
    m_hWnd = hWnd;
    m_nMsg = nMsg;

    m_nbroker = broker;
    m_nusertype = usertype;
    strcpy(m_szuser, pszuser);

    m_pTradeApi->RegisterSpi(this);
    m_pTradeApi->Init();

    m_queueTask.push(TASKID_QRY_ACCOUNT);
    m_queueTask.push(TASKID_QRY_ORDER);
    m_queueTask.push(TASKID_QRY_TRADE);
    m_queueTask.push(TASKID_QRY_POSITION);
    m_bInitOk = false;

    return 1;
}

void            QOrderMgrImpl::Release()
{
    if(m_pTradeApi)
    {
        m_pTradeApi->Release();
    }

    if(m_hdll)
    {
        typedef  void   (__stdcall *pfnReleaseApi)(CXTTradeApi* );
        pfnReleaseApi pfn = (pfnReleaseApi)::GetProcAddress(m_hdll, "ReleaseApi");
        if(pfn)
        {
            pfn(m_pTradeApi);
        }
        m_hdll =0;
    }
    m_pTradeApi =0;

    m_mapTrade.clear();
    m_mapPosition.clear();
    m_arrBranchOrder.clear();
    m_mapSysid2Int.clear();
    m_mapRef2Int.clear();
}

void       QOrderMgrImpl::setHWnd(HWND hWnd, unsigned int nMsg)
{
    m_hWnd = hWnd;
    m_nMsg = nMsg;
}

void        QOrderMgrImpl::SendNotify(WPARAM w, LPARAM l)
{
    if(!m_bInitOk)
    {
        if(m_hWnd)
        {
            PostMessage(m_hWnd, m_nMsg, w, l);
        }
    }

}

void        QOrderMgrImpl::VerifyExCode(tagXTInstrument& inout)
{
    GetDataModule()->VerifyExCode(inout);
}

void       QOrderMgrImpl::WhoIs(int& type, int& broker,  char*  userid  )
{
    broker = m_nbroker;
    type = m_nusertype;
    strcpy(userid, m_szuser);
}

void        QOrderMgrImpl::OnFrontConnected()
{
    qDebug() << "OnFrontConnected";
    SendNotify(10, (LPARAM)this);
}

void        QOrderMgrImpl::OnFrontDisconnected(int nReason)
{
    SendNotify(0, (LPARAM)this);
}

void        QOrderMgrImpl::OnRspUserLogin(tagXTRspInfoField *pRspInfo, bool bIsLast)
{
    if (bIsLast)
    {
        SendNotify(20, (LPARAM)this);

        if(pRspInfo && pRspInfo->ErrorID != 0)
        {
            SendNotify(21, (LPARAM)this);
        }
        else
        {
            inner_doNextTask();
        }
    }
}

void        QOrderMgrImpl::OnRspQryTradingAccount(tagXTTradingAccountField* pAccount, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast)
{
    if (pAccount)
    {
        m_oTradingAccount = *pAccount;
    }

    if (bIsLast)
    {
        inner_doNextTask();
    }
}

void        QOrderMgrImpl::OnRspQryOrder(tagXTOrderField *pOrder, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast)
{
    if (pOrder)
    {
        VerifyExCode(pOrder->ExCode);

        int nPos = inner_existorder(pOrder);
        if( nPos < 0)
        {
            inner_addneworder(pOrder);
        }
        else
        {
            m_arrBranchOrder[nPos] = *pOrder;
        }

    }

    if (bIsLast)
    {
       inner_doNextTask();
    }
}

void        QOrderMgrImpl::OnRspQryTrade(tagXTTradeField *pTrade, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast)
{
    if (pTrade)
    {
        VerifyExCode(pTrade->ExCode);

        string strkey = pTrade->TradeID;
        m_mapTrade.insert( make_pair(strkey, *pTrade));
    }

    if (bIsLast)
    {
        inner_doNextTask();
    }
}

void        QOrderMgrImpl::OnRspQryInvestorPosition(tagXTInvestorPositionField *pIn, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast)
{
    if (pIn)
    {
        qDebug() << "OnRspQryInvestorPosition ExCode = " << pIn->ExCode.Code << " today position " << pIn->TodayPosition << " all position " << pIn->Position
                 << " YdPosition " << pIn->YdPosition << " Direction " << pIn->PosiDirection << " HedgeFlag " << pIn->HedgeFlag <<"PosDate" << pIn->PositionDate;

        VerifyExCode(pIn->ExCode);

        string strkey = inner_getkey(pIn->ExCode.ExchID, pIn->ExCode.Code, pIn->PosiDirection, pIn->HedgeFlag, pIn->PositionDate);

        m_mapOriPosition.insert( make_pair(strkey, *pIn) );
    }

    if (bIsLast)
    {
        if (m_bInitOk)
        {
            return;
        }

        SendNotify(80, (LPARAM)this );

        inner_reArrange();

        SendNotify(100, (LPARAM)this);
        m_bInitOk = true;
    }
}

void        QOrderMgrImpl::OnRspOrderInsert(tagXTInputOrderField *pInputOrder, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast)
{
    if (pRspInfo && pRspInfo->ErrorID != 0)
    {
        QEventCenter* pEventCenter = GetEventCenter();

        OrderEventArgs* e = new OrderEventArgs(CET_Order);
        e->subevent = ORDER_EVENT_FAIL;

        strcpy(e->orderref, pInputOrder->OrderRef);
        strncpy(e->reason, pRspInfo->ErrorMsg, 31);

        e->usertype = m_nusertype;
        e->broker = m_nbroker;
        strcpy(e->user, m_szuser);
        pEventCenter->PostOrderMessage(e);

        string ref(e->orderref);
        m_errorOrderMsg[ref] = pRspInfo->ErrorMsg;
    }
}

string		QOrderMgrImpl::inner_getkey(unsigned char e, const char* pcode, char offset, char  hedge, int posdate /* =0 */)
{
    string strkey;
    if (e == XT_EXCH_SSE || e == XT_EXCH_SZSE)
    {
        char szkey[255]={0};
        if(posdate != 0)
        {
            _snprintf(szkey, 254, "%d_%s_%d_%d_%d", e, pcode, offset, hedge, posdate);
        }
        else
        {
            _snprintf(szkey, 254, "%d_%s_%d_%d", e, pcode, offset, hedge);
        }
        strkey = szkey;
    }
    else
    {
        char szkey[255]={0};
        if(posdate != 0)
        {
            _snprintf(szkey, 254, "0_%s_%d_%d_%d", pcode, offset, hedge, posdate);
        }
        else
        {
            _snprintf(szkey, 254, "0_%s_%d_%d", pcode, offset, hedge);	//CTP的持仓信息中，ExchID没有填
        }
        strkey = szkey;
    }

    return strkey;
}

void        QOrderMgrImpl::OnRtnOrder(tagXTOrderField *pOrder)
{
    if (!m_bInitOk)
    {
        return;		//还没有初始化完
    }

    if (pOrder)
    {
        VerifyExCode(pOrder->ExCode);

        int newtradevol = pOrder->VolumeTraded;
        char newstatus = pOrder->OrderStatus;

        int nPos = inner_existorder(pOrder);
        if (nPos < 0)
        {
            inner_addneworder(pOrder);

            //New
            QEventCenter* pEventCenter = GetEventCenter();
            OrderEventArgs* e = new OrderEventArgs(CET_Order);
            e->subevent = ORDER_EVENT_NEW;
            qDebug() << "New order incoming , " << pOrder->OrderRef;
            strcpy(e->orderref, pOrder->OrderRef);
            e->hastrade = (newtradevol >0);
            e->usertype = m_nusertype;
            e->broker = m_nbroker;
            strcpy(e->user, m_szuser);
            pEventCenter->PostOrderMessage(e);
        }
        else
        {
            tagXTOrderField oldOrder = m_arrBranchOrder[nPos];
            m_arrBranchOrder[nPos] = *pOrder;

            auto curStatus = oldOrder.OrderStatus;
            if (newstatus != curStatus || newtradevol != oldOrder.VolumeTraded)
            {
                //update
                QEventCenter* pEventCenter = GetEventCenter();
                OrderEventArgs* e = new OrderEventArgs(CET_Order);
                e->subevent = ORDER_EVENT_UPDATE;

                strcpy(e->orderref, pOrder->OrderRef);
                e->hastrade = (newtradevol- oldOrder.VolumeTraded) >0;

                e->usertype = m_nusertype;
                e->broker = m_nbroker;
                strcpy(e->user, m_szuser);
                pEventCenter->PostOrderMessage(e);
            }
        }

    }
}

void        QOrderMgrImpl::OnRtnTrade(tagXTTradeField *pTrade)
{
    if (!m_bInitOk)
    {
        return;		//还没有初始化完
    }

    if (pTrade)
    {
        VerifyExCode(pTrade->ExCode);

        bool notify = false;
        string strkey = pTrade->TradeID;
        map<string, tagXTTradeField>::iterator itb = m_mapTrade.find(strkey);
        if (itb == m_mapTrade.end() )
        {
            notify = true;

            //更新持仓
            inner_updateposition(pTrade, true);
        }
        m_mapTrade[strkey] = *pTrade;

        if (notify)
        {
            QEventCenter* pEventCenter = GetEventCenter();
            TransactionEventArgs* e = new TransactionEventArgs(CET_Transact);

            e->subevent = TRANSACT_EVENT_NEW;
            strcpy(e->tradeid, pTrade->TradeID);
            e->usertype = m_nusertype;
            e->broker = m_nbroker;
            strcpy(e->user, m_szuser);
            strcpy(e->orderref, pTrade->OrderRef);
            pEventCenter->PostTransactMessage(e);

        }

    }
}

void		QOrderMgrImpl::inner_reArrange()
{
    //[1]把 原始的Position信息，组合成新的信息(主要是上期所的持仓，2条合并为1条)
    map<string, tagXTInvestorPositionField>::iterator ite = m_mapOriPosition.begin();
    while(ite != m_mapOriPosition.end() )
    {
        tagXTInvestorPositionField& oField = ite->second;
        string strkey = inner_getkey(oField.ExCode.ExchID, oField.ExCode.Code, oField.PosiDirection, oField.HedgeFlag);

        map<string, tagXTInvestorPositionField>::iterator itb = m_mapPosition.find(strkey);
        if(itb == m_mapPosition.end() )
        {
            m_mapPosition.insert( make_pair( strkey, oField));
        }
        else
        {
            tagXTInvestorPositionField& Other = itb->second;
            Other.YdPosition += oField.YdPosition;
            Other.Position += oField.Position;
            Other.TodayPosition += oField.TodayPosition;
            Other.PositionCost += oField.PositionCost;
            Other.PositionProfit += oField.PositionProfit;
        }

        ite++;
    }

    //[2]统计开仓，平仓的数量(根据成交单来统计)
    map<string, tagXTTradeField>::iterator itb = m_mapTrade.begin();
    while (itb != m_mapTrade.end() )
    {
        inner_updateposition( &(itb->second), false);

        itb++;
    }
}

void		QOrderMgrImpl::inner_updateposition(tagXTTradeField *pTrade, bool bpush)
{
    if (pTrade->ExCode.ExchID == XT_EXCH_SSE || pTrade->ExCode.ExchID == XT_EXCH_SZSE)
    {
        //个股期权，股票
    }
    else
    {
        //期货
        inner_updateposition_future(pTrade, bpush);
    }

}

void	QOrderMgrImpl::inner_updateposition_future(tagXTTradeField* pTrade, bool bpush)
{
    //由于期货可能有平今平昨的区别，但是目前没有交易服务器的支持，只能简单的都平
    char  lsflag;
    char  offlag;
    char  hedge = XT_HF_Speculation;
    offlag = pTrade->OffsetFlag;

    if (pTrade->Direction == XT_D_Buy)
    {
        if (pTrade->OffsetFlag == XT_OF_Open)
            lsflag = XT_PD_Long;
        else
            lsflag = XT_PD_Short;
    }
    else
    {
        if (pTrade->OffsetFlag == XT_OF_Open)
            lsflag = XT_PD_Short;
        else
            lsflag = XT_PD_Long;
    }

    string strkey = inner_getkey(pTrade->ExCode.ExchID, pTrade->ExCode.Code, lsflag, hedge);

    //[1]非推送，重整
    if (!bpush)
    {
        map<string, tagXTInvestorPositionField>::iterator itb = m_mapPosition.find(strkey);
        if (itb == m_mapPosition.end() )
        {
            return;
        }
        else
        {
            if (offlag == XT_OF_Open)
            {
                itb->second.OpenVolume += pTrade->Volume;
            }
            else
            {
                itb->second.CloseVolume += pTrade->Volume;
            }
        }

        return;
    }

    //[2]推送
    map<string, tagXTInvestorPositionField>::iterator itb = m_mapPosition.find(strkey);
    if (itb == m_mapPosition.end() )
    {
        tagXTInvestorPositionField  oField;
        memset(&oField, 0, sizeof(oField));
        oField.ExCode.ExchID = pTrade->ExCode.ExchID;
        strcpy(oField.ExCode.Code, pTrade->ExCode.Code);
        oField.PosiDirection = lsflag;
        oField.HedgeFlag = hedge;
        oField.YdPosition =0;
        oField.Position = pTrade->Volume;
        oField.TodayPosition = pTrade->Volume;

        if (offlag == XT_OF_Open)
        {
            oField.OpenVolume = pTrade->Volume;
        }

        m_mapPosition[strkey] = oField;
    }
    else
    {
        if (offlag == XT_OF_Open)
        {
            itb->second.Position += pTrade->Volume;
            itb->second.OpenVolume += pTrade->Volume;
            itb->second.TodayPosition += pTrade->Volume;
        }
        else
        {
            itb->second.Position -= pTrade->Volume;
            itb->second.CloseVolume += pTrade->Volume;

            itb->second.TodayPosition -= pTrade->OffsetFlag == XT_OF_CloseToday ? pTrade->Volume : 0;
        }
    }

}

int		QOrderMgrImpl::InsertOrder(tagXTInputOrderField* pIn)
{
    qDebug() << " Insert order ID " << m_nRequestID;
    int iret =(-1);
    if (m_pTradeApi)
    {
        iret = m_pTradeApi->ReqOrderInsert(pIn, ++m_nRequestID);
    }
    return iret;
}

int		QOrderMgrImpl::CancelOrder(tagXTInputOrderActionField* pIn)
{
    int iret = (-1);
    if (m_pTradeApi)
    {
        iret = m_pTradeApi->ReqOrderAction(pIn, ++m_nRequestID);
    }
    return iret;
}

int		QOrderMgrImpl::GetAllOrder(tagXTOrderField* pArr, int nCount)
{
    int nsize = m_arrBranchOrder.size();
    if (0 == pArr)
    {
        return nsize;
    }

    int nmin = min(nsize, nCount);
    int i =0;
    auto itb = m_arrBranchOrder.begin();
    while (itb != m_arrBranchOrder.end())
    {
        if (i >= nmin)
        {
            break;
        }

        pArr[i] = *itb;

        i++;
        itb++;
    }

    return nmin;
}

int		QOrderMgrImpl::GetAllTrade( tagXTTradeField* pArr, int nCount)
{
    int nsize = m_mapTrade.size();
    if ( 0 == pArr)
    {
        return nsize;
    }

    int nmin = min(nsize, nCount);
    int i=0;
    map<string, tagXTTradeField>::iterator itb = m_mapTrade.begin();
    while (itb != m_mapTrade.end() )
    {
        if (i >= nmin )
        {
            break;
        }
        pArr[i] = itb->second;

        i++;
        itb++;
    }

    return nmin;
}

int		QOrderMgrImpl::GetAllPosition( tagXTInvestorPositionField* pArr, int nCount)
{
    int nsize = m_mapPosition.size();
    if (0 == pArr)
    {
        return nsize;
    }

    int nmin = min(nsize, nCount);
    int i=0;
    map<string, tagXTInvestorPositionField>::iterator itb = m_mapPosition.begin();
    while (itb != m_mapPosition.end() )
    {
        if (i >= nmin)
        {
            break;
        }
        pArr[i] = itb->second;

        i++;
        itb++;
    }

    return nmin;
}


const map<string, string>& QOrderMgrImpl::getErrorMsgsInfo() const
{
    return m_errorOrderMsg;
}

int		QOrderMgrImpl::GetTradingAccount(tagXTTradingAccountField* pIn)
{
    *pIn = m_oTradingAccount;

    return 0;
}


int      QOrderMgrImpl::inner_existorder(tagXTOrderField* pOrder)
{
    if( strlen(pOrder->OrderSysID) >0 )
    {
        string strkey = pOrder->OrderSysID;
        map<string,int>::iterator itb = m_mapSysid2Int.find(strkey);
        if (itb != m_mapSysid2Int.end() )
        {
            return itb->second;
        }
    }

    char szbuf[255] = {0};
    snprintf(szbuf, 254, "%d_%d_%s", pOrder->FrontID, pOrder->SessionID, pOrder->OrderRef );
    string strkey = szbuf;
    map<string, int>::iterator ite = m_mapRef2Int.find(strkey);
    if( ite != m_mapRef2Int.end() )
    {
        return ite->second;
    }

    return (-1);
}

void        QOrderMgrImpl::inner_addneworder(tagXTOrderField* pOrder)
{
    int nNewPos = m_arrBranchOrder.size();
    m_arrBranchOrder.push_back(*pOrder);

    if( strlen(pOrder->OrderSysID) >0 )
    {
        m_mapSysid2Int.insert(make_pair(pOrder->OrderSysID, nNewPos ) );
    }

    char szbuf[255] = {0};
    snprintf(szbuf, 254, "%d_%d_%s", pOrder->FrontID, pOrder->SessionID, pOrder->OrderRef );
    string strkey = szbuf;
    m_mapRef2Int.insert( make_pair(strkey, nNewPos) );
}

void    QOrderMgrImpl::inner_doNextTask()
{
    if(m_queueTask.empty())
    {
        return;
    }

    int taskid = m_queueTask.front();
    m_queueTask.pop();
    if(taskid == TASKID_QRY_ACCOUNT)
    {
        tagXTQryTradingAccountField oField;
        memset(&oField, 0, sizeof(oField));
        int iret = m_pTradeApi->ReqQryTradingAccount(&oField, ++m_nRequestID);
        if (iret >0)
        {
            SendNotify(22, (LPARAM)this);
        }
    }
    else if(taskid == TASKID_QRY_ORDER)
    {
        tagXTQryOrderField oField;
        memset(&oField, 0, sizeof(oField));
        int iret = m_pTradeApi->ReqQryOrder(&oField, ++m_nRequestID);
        if (iret >0)
        {
            SendNotify( 30, (LPARAM)this);
        }
    }
    else if(taskid == TASKID_QRY_TRADE)
    {
        SendNotify( 40, (LPARAM)this);

        tagXTQryTradeField oField;
        memset(&oField, 0, sizeof(oField));
        m_pTradeApi->ReqQryTrade(&oField, ++m_nRequestID);
    }
    else if(taskid == TASKID_QRY_POSITION)
    {
        tagXTQryInvestorPositionField oField;
        memset(&oField, 0, sizeof(oField));

        SendNotify( 50, (LPARAM)this );
        m_pTradeApi->ReqQryInvestorPosition(&oField, ++m_nRequestID);
    }

}

int     QOrderMgrImpl::ReqQryAccount()
{
    m_queueTask.push(TASKID_QRY_ACCOUNT);
    inner_doNextTask();
    return 1;
}

