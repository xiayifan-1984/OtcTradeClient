
#include "ordermgrimpl.h"
#include "datamodule.h"
#include "eventcentermodule.h"
#include "XTCodec.h"
#include "trademoduleimpl.h"
#include "configmodule.h"

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

    m_mapOrder.clear();
    m_mapTrade.clear();
    m_mapPosition.clear();
    m_mapBrachOrder.clear();
}

void       QOrderMgrImpl::setHWnd(HWND hWnd, unsigned int nMsg)
{
    m_hWnd = hWnd;
    m_nMsg = nMsg;
}

void        QOrderMgrImpl::SendNotify(WPARAM w, LPARAM l)
{
    if(m_hWnd)
    {
        PostMessage(m_hWnd, m_nMsg, w, l);
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

//==================================================================================================================================================================================================================================================================================
//==================================================================================================================================================================================================================================================================================
//==================================================================================================================================================================================================================================================================================
//==================================================================================================================================================================================================================================================================================
void        QOrderMgrImpl::OnFrontConnected()
{
    qDebug() << "Front connected";
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

            m_bInitOk = false;

            if (0 == pRspInfo ||  (pRspInfo &&  pRspInfo->ErrorID != 0) )
            {
                qDebug() << "Login error!";
                tagXTQryTradingAccountField oField;
                memset(&oField, 0, sizeof(oField));
                int iret = m_pTradeApi->ReqQryTradingAccount(&oField, ++m_nRequestID);
                if (iret >0)
                {
                    SendNotify(22, (LPARAM)this);
                }
            }
            else
            {
                SendNotify(21, (LPARAM)this);
            }

   }
}

void        QOrderMgrImpl::OnRspQryTradingAccount(tagXTTradingAccountField* pAccount, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast)
{
    if (pAccount)
        {
            m_oTradingAccount = *pAccount;

            if (m_bInitOk)
            {
                /*
                tagSysNotifyEvent oEvent;
                memset(&oEvent, 0, sizeof(oEvent));
                oEvent.mainevent = MAIN_EVENT_ACCOUNT;
                strcpy(oEvent.sval, m_oParam.user);
                GetEventCenter()->PostSysNotifyMessage(&oEvent);
                */
            }

        }

        if (bIsLast)
        {
            if (m_bInitOk)
            {
                return;
            }

            tagXTQryOrderField oField;
            memset(&oField, 0, sizeof(oField));
            int iret = m_pTradeApi->ReqQryOrder(&oField, ++m_nRequestID);
            if (iret >0)
            {
                SendNotify( 30, (LPARAM)this);
            }
        }
}

void        QOrderMgrImpl::OnRspQryOrder(tagXTOrderField *pOrder, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast)
{
    if (pOrder)
        {
            VerifyExCode(pOrder->ExCode);
            string strkey = pOrder->OrderRef;
            if (strkey.length() >0)
            {
                m_mapBrachOrder.insert(make_pair(strkey, *pOrder));

                m_mapOrder.insert( make_pair(strkey, strkey) );
            }
        }

        if (bIsLast)
        {
            if (m_bInitOk)
            {
                return;
            }

            SendNotify( 40, (LPARAM)this);

            tagXTQryTradeField oField;
            memset(&oField, 0, sizeof(oField));
            m_pTradeApi->ReqQryTrade(&oField, ++m_nRequestID);
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
            if (m_bInitOk)
            {
                return;
            }

            tagXTQryInvestorPositionField oField;
            memset(&oField, 0, sizeof(oField));

            SendNotify( 50, (LPARAM)this );
            m_pTradeApi->ReqQryInvestorPosition(&oField, ++m_nRequestID);
        }
}

void        QOrderMgrImpl::OnRspQryInvestorPosition(tagXTInvestorPositionField *pIn, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast)
{
    if (pIn)
    {
            qDebug() << "ExCode = " << pIn->ExCode.Code << " today position " << pIn->TodayPosition << " all position " << pIn->Position << " reqId " << nRequestID ;
            VerifyExCode(pIn->ExCode);

            string strkey = inner_getkey(pIn->ExCode.ExchID, pIn->ExCode.Code, pIn->PosiDirection, pIn->HedgeFlag);

            m_mapPosition.insert(make_pair(strkey, *pIn));
    }

        if (bIsLast)
        {
            if (m_bInitOk)
            {
                return;
            }

            SendNotify(80, (LPARAM)this );

            inner_reArrange();

            m_bInitOk = true;
            SendNotify(100, (LPARAM)this);
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
    }
}

string		QOrderMgrImpl::inner_getkey(unsigned char e, const char* pcode, char offset, char  hedge)
{
    string strkey;
    if (e == XT_EXCH_SSE || e == XT_EXCH_SZSE)
    {
        char szkey[255]={0};
        _snprintf(szkey, 254, "%d_%s_%d_%d", e, pcode, offset, hedge);
        strkey = szkey;
    }
    else
    {
        char szkey[255]={0};
        _snprintf(szkey, 254, "0_%s_%d_%d", pcode, offset, hedge);	//CTP的持仓信息中，ExchID没有填
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

        string strkey = pOrder->OrderRef;

        int newtradevol = pOrder->VolumeTraded;
        char newstatus = pOrder->OrderStatus;

        auto itb = m_mapBrachOrder.find(strkey);
        qDebug() << "order key = " << pOrder->OrderRef;
        if (itb == m_mapBrachOrder.end())
        {
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
            //Compare
            qDebug() << "order updating , " << pOrder->OrderRef << " ,new status = " << newstatus << " , curStatus = "<<itb->second.OrderStatus;
            if (newstatus != itb->second.OrderStatus || newtradevol != itb->second.VolumeTraded)
            {
                //update
                QEventCenter* pEventCenter = GetEventCenter();
                OrderEventArgs* e = new OrderEventArgs(CET_Order);
                e->subevent = ORDER_EVENT_UPDATE;
                qDebug() << "order updating coming!";
                strcpy(e->orderref, pOrder->OrderRef);
                e->hastrade = (newtradevol- itb->second.VolumeTraded) >0;

                e->usertype = m_nusertype;
                e->broker = m_nbroker;
                strcpy(e->user, m_szuser);
                pEventCenter->PostOrderMessage(e);
            }
        }
        m_mapBrachOrder[strkey] = *pOrder;
        m_mapOrder[strkey] = strkey;

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

        //tagXTQryInvestorPositionField oField;
        //memset(&oField, 0, sizeof(oField));

       // SendNotify( 50, (LPARAM)this );
       // m_pTradeApi->ReqQryInvestorPosition(&oField, ++m_nRequestID);
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
            qDebug() << "pTrade offset " << pTrade->OffsetFlag;
            itb->second.TodayPosition -= pTrade->OffsetFlag == XT_OF_CloseToday ? pTrade->Volume : 0;
        }
    }

}

//==================================================================================================================================================================================================================================================================================
//==================================================================================================================================================================================================================================================================================
//==================================================================================================================================================================================================================================================================================
//==================================================================================================================================================================================================================================================================================
int		QOrderMgrImpl::InsertOrder(tagXTInputOrderField* pIn)
{
    /*
    GetLogger(LOGGER_DEV_PROGRAM)->WriteInfo(0, "InsertOrder[orderRef=%s][code=%s][dir=%c][of=%c][pt=%c][price=%.04lf][vol=%d][hedge=%c]",
        pIn->OrderRef, pIn->ExCode.Code, pIn->Direction,pIn->OffsetFlag,
        pIn->PriceType,pIn->LimitPrice,pIn->Volume, pIn->HedgeFlag
        );
    */
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
    /*
    GetLogger(LOGGER_DEV_PROGRAM)->WriteInfo(0, "CancelOrder[orderRef=%s][code=%s][fid=%d][sid=%d][sysid=%s]",
        pIn->OrderRef, pIn->ExCode.Code, pIn->FrontID, pIn->SessionID, pIn->OrderSysID
        );
    */

    int iret = (-1);
    if (m_pTradeApi)
    {
        iret = m_pTradeApi->ReqOrderAction(pIn, ++m_nRequestID);
    }
    return iret;
}


int		QOrderMgrImpl::GetAllOrder(tagXTOrderField* pArr, int nCount)
{
    int nsize = m_mapBrachOrder.size();
    if (0 == pArr)
    {
        return nsize;
    }

    int nmin = min(nsize, nCount);
    int i =0;
    auto itb = m_mapBrachOrder.begin();
    while (itb != m_mapBrachOrder.end())
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

int		QOrderMgrImpl::GetOneOrder(const char* pszOrderRef, tagXTOrderField& oOut)
{
    int iret = -1;
    string strkey = pszOrderRef;
    auto itb = m_mapOrder.find(strkey);
    if (itb != m_mapOrder.end() )
    {
        iret =1;

        oOut = m_mapBrachOrder[itb->second];
    }
    return iret;
}

int		QOrderMgrImpl::GetOnePosition(tagXTInstrument& oExCode,char LongShort, char  HedgeFlag, tagXTInvestorPositionField& oOut)
{
    int iret = -1;

    string strkey = inner_getkey(oExCode.ExchID, oExCode.Code, LongShort, HedgeFlag);
    map<string, tagXTInvestorPositionField>::iterator itb = m_mapPosition.find(strkey);
    if (itb != m_mapPosition.end() )
    {
        oOut = itb->second;
        iret = 1;
    }

    return iret;
}

void  QOrderMgrImpl::GetOrdersForInstrument(tagXTInstrument& oExCode, std::vector<tagXTOrderField>& oOut)
{
   auto itb = m_mapBrachOrder.begin();
   auto len = strlen(oExCode.Code);
   while(itb != m_mapBrachOrder.end())
   {
        auto&  u = itb->second;
        if(0 == _strnicmp(oExCode.Code, u.ExCode.Code, len) and u.ExCode.ExchID ==oExCode.ExchID)
        {
            oOut.push_back(u);
        }
        ++itb;
    }
}

void QOrderMgrImpl::GetPositionsForInstrument(tagXTInstrument& oExCode, std::vector<tagXTInvestorPositionField>& oOut)
{
    auto itb = m_mapPosition.begin();
    auto len = strlen(oExCode.Code);
    while(itb != m_mapPosition.end())
    {
        auto& u= itb->second;
        if(0 == _strnicmp(oExCode.Code, u.ExCode.Code, len) and u.ExCode.ExchID == oExCode.ExchID)
        {
            oOut.push_back(u);
        }
        ++itb;
    }
}

int		QOrderMgrImpl::GetTradingAccount(tagXTTradingAccountField* pIn)
{
    *pIn = m_oTradingAccount;

    /*
    if (m_oTradingAccount.broker == 2011)
    {
        return  inner_gethuabao(pIn);
    }
    else if (m_oTradingAccount.broker == 8200)
    {
        if (m_oTradingAccount.type == 4)
        {
            return inner_xzfuture(pIn);
        }
        else
        {
            return inner_xz(pIn);
        }
    }
    */

    return 0;
}

QOrderMgr* getOrderMgrbyInstrument(tagXTInstrument& curCode)
{
    auto pTradeMod = static_cast<QTradeModuleImpl*>(GetTradeModule());
    if(!pTradeMod) return nullptr;

    QExchange* pExchange = GetDataModule()->GetExchange(curCode.ExchID);
    if(!pExchange) return nullptr;

    tagXTNameTable oNameCode;
    pExchange->GetOneNameTable(&curCode, &oNameCode);
    qDebug()<< "Exchange = " << curCode.ExchID << " instrument name " << oNameCode.Name;

    vector<tagOneTradeUser>  arrUser;
    GetConfigModule()->GetTradeUserBy(oNameCode.ProductID, arrUser);
    qDebug() << "Users number = " << arrUser.size();
    auto pMgr = arrUser.size()>0 ? pTradeMod->GetMgr(arrUser[0].type, arrUser[0].broker, arrUser[0].user): nullptr;
    return pMgr;
}

