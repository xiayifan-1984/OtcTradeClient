#include "algoordermgr.h"
#include <string.h>
#include "rpcstream.h"
#include "internalmessaging.h"
#include "stool.h"
#include "eventcentermodule.h"
#include "ordermgrimpl.h"
#include <algorithm>

using namespace  std;

AlgoOrderMgr* g_algoOrderMgr = nullptr;
AlgoOrderMgr* GetAlgoOrderMgr()
{
    if(g_algoOrderMgr)
    {
        return g_algoOrderMgr;
    }
    g_algoOrderMgr = new AlgoOrderMgr();
    return g_algoOrderMgr;
}

AlgoOrderMgrByUser::AlgoOrderMgrByUser(const tagOneTradeUser& user)
{
    memset(&m_user,0, sizeof(m_user));
    memcpy(&m_user, &user, sizeof(m_user));
    m_reqCount = 0;
}

int AlgoOrderMgrByUser::insertAlgoOrder(tagXTAlgoOrderField *inputOrder)
{
    if(!inputOrder) return -1;
    char tmpBuf[4096]{0};
    RpcWrite rw(tmpBuf, 4095);
    rw.WriteInt32(2);
    rw.WriteSolidPtr((char*)inputOrder, sizeof(tagXTAlgoOrderField));
    rw.WriteInt32(++m_reqCount);
    if(!rw.IsError())
    {
        updateOrderInfo(inputOrder);
        return sendRequest(2, 21, tmpBuf, rw.GetOffset(), m_user.broker, m_user.user);
    }
}

int AlgoOrderMgrByUser::onAlgoOrderRtn(tagXTAlgoOrderField *input)
{
    if(!input) return -1;
    if(input->AlgoOrderType < XT_ALGO_TWAP || input->AlgoOrderType >= XT_ALGO_INVALID)
    {
        return -1;
    }
    string ref = input->AlgoOrderID;
    auto search = m_algoOrderCtx[input->AlgoOrderType].find(ref);
    if(search == m_algoOrderCtx[input->AlgoOrderType].end())
    {
        return 0;
    }
    m_algoOrderCtx[input->AlgoOrderType][ref].AlgoOrderStatus = input->AlgoOrderStatus;

    QEventCenter* pEventCenter = GetEventCenter();
    AlgoOrderEvent* e = new AlgoOrderEvent(CET_AlgoTwap);
    e->usertype = 0;
    e->subevent = ORDER_EVENT_UPDATE;
    e->broker = input->BrokerID;
    strncpy(e->user, input->UserID, strlen(input->UserID));
    qDebug() << "New algo twap order incoming , " << input->AlgoOrderID;
    pEventCenter->PostAlgoOrderMessage(e);
    return  1;
}

int AlgoOrderMgrByUser::onAlgoRelatedOrderRtn(tagXTAlgoOrderInsertRtnField *input)
{
    if(!input) return -1;
    if(input->AlgoOrderType < XT_ALGO_TWAP || input->AlgoOrderType >= XT_ALGO_INVALID)
    {
        return -1;
    }
    string ref = input->AlgoOrderID;
    m_relativeOrdersRecord[ref].push_back(input->InsertOrderRef);

    QEventCenter* pEventCenter = GetEventCenter();
    AlgoOrderEvent* e = new AlgoOrderEvent(CET_AlgoTwap);
    e->usertype = 0;
    e->subevent = ORDER_EVENT_UPDATE;
    e->broker = input->BrokerID;
    strncpy(e->user, input->UserID, strlen(input->UserID));
    qDebug() << "New algo twap Relative order incoming , " << input->AlgoOrderID;
    pEventCenter->PostAlgoOrderMessage(e);
    return  1;

    return 1;
}

int AlgoOrderMgrByUser::onAlgoOrderQryRtn(tagXTQryAlgoOrderField *input, tagXTRspInfoField *pRspInf, int reqId, bool isLast)
{
    if(!input) return -1;
    string algoRef = input->AlgoOrderID;
    auto algoType = input->AlgoOrderType;

    auto& algoCtx = m_algoOrderCtx[algoType];
    algoCtx[algoRef] = *(dynamic_cast<tagXTAlgoOrderField*>(input));
    for(int i = 0; i < input->CurOrderNum; ++i)
    {
        string orderRef(input->CurOrderRef[i]);
        m_relativeOrdersRecord[algoRef].push_back(orderRef);
    }
    return 1;
}

int AlgoOrderMgrByUser::queryAlgoOrder(tagXTQryAlgoOrderField *input)
{
    if(!input) return -1;
    char tmpBuf[4096]{0};
    RpcWrite rw(tmpBuf, 4095);
    rw.WriteInt32(2);
    rw.WriteSolidPtr((char*)input, sizeof(tagXTQryAlgoOrderField));
    rw.WriteInt32(++m_reqCount);
    if(!rw.IsError())
    {
        return sendRequest(2, 20, tmpBuf, rw.GetOffset(), m_user.broker, m_user.user);
    }
    return -1;
}

QString AlgoOrderMgrByUser::getErrorMsg(const string &orderRef, XTAlgoOrderType ty) const
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

int AlgoOrderMgrByUser::cancelAlgoOrder(tagXTAlgoOrderActionField *inputOrder)
{
    if(!inputOrder) return -1;
    char tmpBuf[4096]{0};
    RpcWrite rw(tmpBuf, 4095);
    rw.WriteInt32(2);
    rw.WriteSolidPtr((char*)inputOrder, sizeof(tagXTAlgoOrderField));
    rw.WriteInt32(++m_reqCount);
    if(!rw.IsError())
    {
        return sendRequest(2, 22, tmpBuf, rw.GetOffset(), m_user.broker, m_user.user);
    }
}

AlgoRelationOrders &AlgoOrderMgrByUser::getAlgoRef2OrderRefMap()
{
    return m_relativeOrdersRecord;
}

int AlgoOrderMgrByUser::cancelOrder(const string &ref, XTAlgoOrderType twTy)
{
    tagXTAlgoOrderActionField oField;
    memset(&oField, 0, sizeof(oField));
    const auto&twOrders = m_algoOrderCtx[twTy];
    auto search = twOrders.find(ref);
    if (search == twOrders.end())
    {
        return 0;
    }
    const auto& algoOrder = search ->second;
    if(algoOrder.AlgoOrderStatus == XT_ALGO_INVALID || algoOrder.AlgoOrderStatus == XT_AlGO_Cancelled || algoOrder.AlgoOrderStatus == XT_AlGO_End)
    {
        return 1;
    }
    oField.AlgoOrderType = twTy;
    strncpy(oField.AlgoOrderID, algoOrder.AlgoOrderID, sizeof(oField.AlgoOrderID));
    oField.BrokerID = algoOrder.BrokerID;
    strncpy(oField.UserID, algoOrder.UserID, sizeof(oField.UserID));
    oField.ExCode.ExchID = algoOrder.ExCode.ExchID;
    strncpy(oField.ExCode.Code, algoOrder.ExCode.Code, sizeof(oField.ExCode.Code));
    cancelAlgoOrder(&oField);
    return 1;
}

std::vector<std::pair<AlgoOrderRef, tagXTAlgoOrderField>> AlgoOrderMgrByUser::getAlgoOrders()
{
    std::vector<std::pair<AlgoOrderRef, tagXTAlgoOrderField>> result;
    for(auto& ele:m_algoOrderCtx)
    {
        for(auto& p:ele.second)
        {
            result.push_back(p);
        }
    }
    return move(result);
}

int AlgoOrderMgrByUser::tradedOrders(const tagXTAlgoOrderField& twOrder) const
{
   auto search = m_relativeOrdersRecord.find(twOrder.AlgoOrderID);
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

int AlgoOrderMgrByUser::sendRequest(unsigned char mainType, unsigned char childType, const char *inbuf, unsigned short insize, int broker, const char *userId)
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
    auto pProducer = GetInternalMsgSenderReceiver();
    if(pProducer)
    {
        pProducer->sendMsg("430", tempbuf, sizeof(tagTDUserInfo) + sizeof(tagComm_FrameHead)  + pHead->size);
        return   sizeof(tagTDUserInfo) + sizeof(tagComm_FrameHead)  + pHead->size  ;
    }
    return  -1;
}

void AlgoOrderMgrByUser::updateOrderInfo(tagXTAlgoOrderField *input)
{
    string ref(input->AlgoOrderID);
    m_algoOrderCtx[input->AlgoOrderType][ref] = *input;
}

int AlgoOrderMgr::initUsers()
{
    auto pConf = GetConfigModule();
    if(pConf)
    {
        lock_guard<mutex> lock(mut);
        vector<tagOneTradeUser> users;
        auto size = pConf->GetAllTradeUser(users);
        if(size > 0)
        {
            for(auto& ele:users)
            {
                char key[255]{0};
                sprintf(key, "%d_%s", ele.broker, ele.user);
                string strkey(key);
                m_algoOrderMgr.emplace(key, move(ele));

                //string userRef(key);
                tagXTQryAlgoOrderField qry;
                memset(&qry, 0, sizeof(tagXTQryAlgoOrderField));
                //m_algoOrderMgr[strkey].queryAlgoOrder(&qry);
                auto search = m_algoOrderMgr.find(strkey);
                if(search != m_algoOrderMgr.end())
                {
                    search->second.queryAlgoOrder(&qry);
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

bool AlgoOrderMgr::handleBuf(const char *pbuf, int buflen)
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
        case 20:
        {
            int argc = rr.ReadInt32();
            tagXTQryAlgoOrderField* pField = (tagXTQryAlgoOrderField*)rr.ReadSolidPtr();
           // tagXTQryAlgoOrderField* pRspInfo = (tagXTQryAlgoOrderField*)rr.ReadSolidPtr();
           // int reqId = rr.ReadInt32();
            //bool bLast = rr.ReadInt8()>0;
            if(rr.IsError())
            {
                printf("onAlgoOrderQryRtn failed\n");
            }
            else
            {
                for(int i = 0; i < pField->CurOrderNum; ++i)
                {
                    qDebug() << "AlgoRef = " << pField->AlgoOrderID << ", orderRef = " << pField->CurOrderRef[i];
                }
                  auto strkey = stool::genUserId(userInfo.broker, userInfo.userid);
                  auto algoUser = findOrderMgr(strkey);
                  if(algoUser)
                  {
                    //    parkUser->onRspQryParkedOrder(pField, pRspInfo, reqId, bLast);
                      algoUser->onAlgoOrderQryRtn(pField, nullptr, 0, false);
                  }
            }
        }
        break;

        case 23:
        {
            int argc = rr.ReadInt32();
            tagXTAlgoOrderField* pField = (tagXTAlgoOrderField*)rr.ReadSolidPtr();
            if(rr.IsError())
            {
                printf("OnRtnAlgoOrder failed\n");
            }
            else
            {
                auto strkey = stool::genUserId(userInfo.broker, userInfo.userid);
                auto algoUser = findOrderMgr(strkey);
                if(algoUser)
                {
                    algoUser->onAlgoOrderRtn(pField);
                }
            }
        }
        break;

        case 24:
        {
            int argc = rr.ReadInt32();
            tagXTAlgoOrderInsertRtnField* pField = (tagXTAlgoOrderInsertRtnField*)rr.ReadSolidPtr();
            if(rr.IsError())
            {
                printf("OnRtnAlgoOrder failed\n");
            }
            else
            {
                auto strkey = stool::genUserId(userInfo.broker, userInfo.userid);
                auto algoUser = findOrderMgr(strkey);
                if(algoUser)
                {
                    algoUser->onAlgoRelatedOrderRtn(pField);
                }
            }
        }
        break;

    }
    return !rr.IsError();
}

AlgoOrderMgrByUser *AlgoOrderMgr::findOrderMgr(const string &userRef)
{
    auto search = m_algoOrderMgr.find(userRef);
    if(search != m_algoOrderMgr.end())
    {
        return &(search->second);
    }
    return nullptr;
}
