#include "twapordermgr.h"
#include <string.h>
#include "rpcstream.h"
#include "kafkamsgunit.h"
#include "stool.h"
#include "eventcentermodule.h"
#include "trademodule.h"
#include <algorithm>

using namespace  std;

TwapOrderMgr* g_twapOrderMgr = nullptr;
TwapOrderMgr* GetTwapOrderMgr()
{
    if(g_twapOrderMgr)
    {
        return g_twapOrderMgr;
    }
    g_twapOrderMgr = new TwapOrderMgr();
    return g_twapOrderMgr;
}

TwapOrderMgrByUser::TwapOrderMgrByUser(const tagTTradeUserParam& user)
{
    memset(&m_user,0, sizeof(m_user));
    memcpy(&m_user, &user, sizeof(m_user));
    m_reqCount = 0;
}

int TwapOrderMgrByUser::DoReqTwapOrderInsert(tagXTTwapInputOrderField *inputOrder)
{
    if(!inputOrder) return -1;
    char tmpBuf[4096] = {0};

    RpcWrite rw(tmpBuf, 4095);
    rw.WriteInt32(2);
    rw.WriteSolidPtr((char*)inputOrder, sizeof(tagXTTwapInputOrderField));
    rw.WriteInt32(++m_reqCount);
    if(!rw.IsError())
    {
        updateOrderInfo(inputOrder);
        return sendRequest(2, 41, tmpBuf, rw.GetOffset(), m_user.broker, m_user.user);
    }
}

int TwapOrderMgrByUser::cancelTwapOrder(tagXTTwapOrderActionField *inputOrder)
{
    if(!inputOrder) return -1;
    char tmpBuf[4096]{0};
    RpcWrite rw(tmpBuf, 4095);
    rw.WriteInt32(2);
    rw.WriteSolidPtr((char*)inputOrder, sizeof(tagXTTwapOrderActionField));
    rw.WriteInt32(++m_reqCount);
    if(!rw.IsError())
    {
        return sendRequest(2, 42, tmpBuf, rw.GetOffset(), m_user.broker, m_user.user);
    }
}

int TwapOrderMgrByUser::DoReqTwapOrderAction(const string& twapid, char twapType)
{
    const auto&twOrders = m_twapOrderCtx[twapType];
    auto search = twOrders.find(twapid);
    if (search == twOrders.end())
    {
        return 0;
    }
    const auto& twapOrder = search ->second;
    if(twapOrder.TwapOrderStatus == XT_TWAP_End || twapOrder.TwapOrderStatus == XT_TWAP_Cancelled || twapOrder.TwapOrderStatus == XT_TWAP_AVOID)
    {
        return 1;
    }

    tagXTTwapOrderActionField oField;
    memset(&oField, 0, sizeof(tagXTTwapOrderActionField));
    oField.BrokerID = twapOrder.BrokerID;
    strncpy(oField.UserID, twapOrder.UserID, sizeof(oField.UserID));
    strncpy(oField.TwapOrderID, twapOrder.TwapOrderID, sizeof(oField.TwapOrderID));
    oField.ActionFlag = XT_AF_Delete;

    cancelTwapOrder(&oField);
    
    return 1;
}

int TwapOrderMgrByUser::DoReqQryTwapOrder(tagXTTwapQryOrderField *input)
{
    if(!input) return -1;

    char tmpBuf[4096]{0};
    RpcWrite rw(tmpBuf, 4095);
    rw.WriteInt32(2);
    rw.WriteSolidPtr((char*)input, sizeof(tagXTTwapQryOrderField));
    rw.WriteInt32(++m_reqCount);
    if(!rw.IsError())
    {
        //此处2个请求
        sendRequest(2, 40, tmpBuf, rw.GetOffset(), m_user.broker, m_user.user);
        sendRequest(2, 43, tmpBuf, rw.GetOffset(), m_user.broker, m_user.user);
        return 1;
    }
    return -1;
}

int TwapOrderMgrByUser::OnRspQryTwapOrder(tagXTTwapOrderField* input, tagXTRspInfoField* pRspInf, int reqId, bool isLast)
{
    if(!input) return -1;

    string twapRef = input->TwapOrderID;
    auto twapType = input->TwapOrderType;

    auto& twapCtx = m_twapOrderCtx[twapType];
    twapCtx[twapRef] = *input;
    
    return 1;
}

int TwapOrderMgrByUser::sendRequest(unsigned char mainType, unsigned char childType, const char *inbuf, unsigned short insize, int broker, const char *userId)
{
    char tempbuf[4096]={0};
    tagTDUserInfo* pUser = (tagTDUserInfo*)(tempbuf);
    pUser->broker = broker;
    if (userId)
    {
        strncpy(pUser->userid, userId, strlen(userId));
    }

    tagComm_FrameHead* pHead = (tagComm_FrameHead*)(tempbuf + sizeof(tagTDUserInfo));
    pHead->mainType = mainType;
    pHead->childType = childType;
    pHead->size = insize;

    if(insize >0)
    {
        memcpy(tempbuf + sizeof(tagTDUserInfo) + sizeof(tagComm_FrameHead), inbuf, insize);
    }

    //加入队列
    auto pProducer = GetKafkaUnit();
    if(pProducer)
    {
        pProducer->sendMsg("430", tempbuf, sizeof(tagTDUserInfo) + sizeof(tagComm_FrameHead)  + pHead->size);
        return   sizeof(tagTDUserInfo) + sizeof(tagComm_FrameHead)  + pHead->size  ;
    }
    return  -1;
}

void TwapOrderMgrByUser::updateOrderInfo(tagXTTwapInputOrderField *input)
{
    string ref(input->TwapOrderID);
    tagXTTwapOrderField oField;
    memset(&oField, 0, sizeof(oField));
    memcpy(&oField, input, sizeof(tagXTTwapInputOrderField));

    m_twapOrderCtx[input->TwapOrderType][ref] = oField;
}

int TwapOrderMgrByUser::OnTwapOrderRtn(tagXTTwapOrderField* input)
{
    if(!input)
    {
        return -1;
    }

    string twapid = input->TwapOrderID;
    m_twapOrderCtx[input->TwapOrderType][twapid] = *input;

    QEventCenter* pEventCenter = GetEventCenter();
    TwapOrderEvent* e = new TwapOrderEvent(CET_TwapTwap);
    e->usertype = 0;
    e->subevent = ORDER_EVENT_UPDATE;
    e->broker = input->BrokerID;
    strncpy(e->user, input->UserID, strlen(input->UserID));
    qDebug() << "OnTwapOrderRtn " << input->TwapOrderID;
    pEventCenter->PostTwapOrderMessage(e);
    return  1;
}

std::vector<std::pair<TwapOrderRef, tagXTTwapOrderField>> TwapOrderMgrByUser::getAllTwapOrders()
{
    std::vector<std::pair<TwapOrderRef, tagXTTwapOrderField>> result;
    for(auto& ele:m_twapOrderCtx)
    {
        for(auto& p:ele.second)
        {
            result.push_back(p);
        }
    }
    return move(result);
}

int         TwapOrderMgrByUser::OnTwapOrderInsertRtn(tagXTTwapOrderInsertRtnField* input)
{
    if(!input) return -1;

    string twapid = input->TwapOrderID;
    string realRef = input->RealOrderRef;
    m_relativeOrdersRecord[twapid].insert(realRef);
    
    QEventCenter* pEventCenter = GetEventCenter();
    TwapOrderEvent* e = new TwapOrderEvent(CET_TwapTwap);
    e->usertype = 0;
    e->subevent = ORDER_EVENT_UPDATE;
    e->broker = input->BrokerID;
    strncpy(e->user, input->UserID, strlen(input->UserID));
    qDebug() << "OnTwapOrderInsertRtn " << input->TwapOrderID;
    pEventCenter->PostTwapOrderMessage(e);
    return  1;
}

int         TwapOrderMgrByUser::OnRspQryTwapOrderDetail(tagXTTwapOrderInsertRtnField* input, tagXTRspInfoField* pRspInf, int reqId, bool isLast)
{
    if(!input) return -1;

    string twapid = input->TwapOrderID;
    string realRef = input->RealOrderRef;
    m_relativeOrdersRecord[twapid].insert(realRef);
}

QString TwapOrderMgrByUser::getErrorMsg(const string &orderRef, char twaptype) const
{
    QString result;
    QOrderMgr* p = GetTradeModule()->GetMgr(m_user.type, m_user.broker, m_user.user);
    if(!p) return result;
    const auto& errorMsgInfo = p->getErrorMsgsInfo();
    for(const auto& p:m_relativeOrdersRecord)
    {
        const auto& orderRefs = p.second;
        for(const auto& ref:orderRefs)
        {
            auto search = errorMsgInfo.find(ref);
            if(search != errorMsgInfo.end() && orderRef == p.first)
            {
                result = search->second.c_str();
                return result;
            }
        }
    }
    return result;
}

TwapRelationOrders &TwapOrderMgrByUser::getTwapRef2OrderRefMap()
{
    return m_relativeOrdersRecord;
}

int TwapOrderMgrByUser::tradedOrders(const std::string& twapid) const
{
   auto search = m_relativeOrdersRecord.find(twapid);
   if(search == m_relativeOrdersRecord.end())
   {
       return 0;
   }
   const auto& orderRefs = search->second;
   QOrderMgr* p = GetTradeModule()->GetMgr(m_user.type, m_user.broker, m_user.user);
   if(!p) return 0;

   int ncount = p->GetAllOrder(nullptr, 0);
   int result = 0;
   if (ncount > 0)
   {
       tagXTOrderField* pArr = new tagXTOrderField[ncount];
       p->GetAllOrder(pArr, ncount);

       for(int i=0; i < ncount; ++i)
       {
           if(pArr[i].OrderStatus == XT_OST_Traded)
           {
               string key = pArr[i].OrderRef;
               auto search = find_if(orderRefs.begin(), orderRefs.end(), [&](const string& s){
                       return s == key;});
               if(search != orderRefs.end())
               {
                   result += pArr[i].VolumeTraded;
               }
           }
       }
       delete[] pArr;
   }
   return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int TwapOrderMgr::initUsers()
{
    auto pConf = GetConfigModule();
    if(pConf)
    {
        lock_guard<mutex> lock(mut);
        vector<tagTTradeUserParam> users;
        auto size = pConf->GetAllTradeUser(users);
        if(size > 0)
        {
            for(auto& ele:users)
            {
                char key[255]{0};
                sprintf(key, "%d_%s", ele.broker, ele.user);
                string strkey(key);

                shared_ptr<TwapOrderMgrByUser> p =  make_shared<TwapOrderMgrByUser>(ele);
                m_mapUser2Ptr.insert({strkey, p});

                tagXTTwapQryOrderField  oField;
                memset(&oField, 0, sizeof(tagXTTwapQryOrderField));
                oField.BrokerID = ele.broker;
                strcpy(oField.UserID, ele.user);
                
                auto search = m_mapUser2Ptr.find(strkey);
                if(search != m_mapUser2Ptr.end())
                {
                    search->second->DoReqQryTwapOrder(&oField);
                }
            }
        }
        cond.notify_one();
        return 1;
    }
    else
    {
        return -1;
    }
}

bool TwapOrderMgr::handleBuf(const char *pbuf, int buflen)
{
    unique_lock<mutex> ulock(mut);
    cond.wait(ulock, [](){return true;});
    tagTDUserInfo userInfo;
    memcpy(&userInfo, pbuf, sizeof(tagTDUserInfo));

    buflen -= sizeof(tagTDUserInfo);

    if(buflen <= sizeof(tagComm_FrameHead))
    {
        return false;
    }

    tagComm_FrameHead head;
    memcpy(&head, pbuf+sizeof(tagTDUserInfo), sizeof(tagComm_FrameHead));

    buflen -= sizeof(tagComm_FrameHead);

    if(buflen < head.size)
    {
        return false;
    }

    RpcRead rr;
    rr.BindStream((const_cast<char*>(pbuf))+sizeof(tagComm_FrameHead)+sizeof(tagTDUserInfo), buflen);
    switch(head.childType)
    {
        case 40:
        {
            int argc = rr.ReadInt32();
            tagXTTwapOrderField* pField = (tagXTTwapOrderField*)rr.ReadSolidPtr();
            tagXTRspInfoField* pRspInfo = (tagXTRspInfoField*)rr.ReadSolidPtr();
            int reqId = rr.ReadInt32();
            bool bLast = rr.ReadInt8()>0;
            if(rr.IsError())
            {
                qDebug() << "OnRspQryTwapOrder failed";
            }
            else
            {
                auto strkey = stool::genUserId(userInfo.broker, userInfo.userid);
                auto twapUser = findOrderMgr(strkey);
                if(twapUser)
                {
                    twapUser->OnRspQryTwapOrder(pField, pRspInfo, reqId, bLast);
                }
            }
        }
        break;
        case 43:
        {
            int argc = rr.ReadInt32();
            tagXTTwapOrderInsertRtnField * pField = (tagXTTwapOrderInsertRtnField *)rr.ReadSolidPtr();
            tagXTRspInfoField* pRspInfo = (tagXTRspInfoField*)rr.ReadSolidPtr();
            int reqId = rr.ReadInt32();
            bool bLast = rr.ReadInt8()>0;
            if(rr.IsError())
            {
                qDebug() << "OnRspQryTwapOrderDetail failed";
            }
            else
            {
                auto strkey = stool::genUserId(userInfo.broker, userInfo.userid);
                auto twapUser = findOrderMgr(strkey);
                if(twapUser)
                {
                    twapUser->OnRspQryTwapOrderDetail(pField, pRspInfo, reqId, bLast);
                }
            }
        }
        break;
        case 45:
        {
            int argc = rr.ReadInt32();
            tagXTTwapOrderField* pField = (tagXTTwapOrderField*)rr.ReadSolidPtr();
            if(rr.IsError())
            {
                qDebug() << "OnTwapOrderRtn failed";
            }
            else
            {
                auto strkey = stool::genUserId(userInfo.broker, userInfo.userid);
                auto twapUser = findOrderMgr(strkey);
                if(twapUser)
                {
                    twapUser->OnTwapOrderRtn(pField);
                }
            }
        }
        break;
        case 46:
        {
            int argc = rr.ReadInt32();
            tagXTTwapOrderInsertRtnField* pField = (tagXTTwapOrderInsertRtnField*)rr.ReadSolidPtr();
            if(rr.IsError())
            {
                qDebug() << "OnTwapOrderInsertRtn failed\n";
            }
            else
            {
                auto strkey = stool::genUserId(userInfo.broker, userInfo.userid);
                auto twapUser = findOrderMgr(strkey);
                if(twapUser)
                {
                    twapUser->OnTwapOrderInsertRtn(pField);
                }
            }
        }
        break;

    }
    return !rr.IsError();
}

TwapOrderMgrByUser* TwapOrderMgr::findOrderMgr(const string &userRef)
{
    auto search = m_mapUser2Ptr.find(userRef);
    if(search != m_mapUser2Ptr.end())
    {
        return search->second.get();
    }
    return nullptr;
}
