#include "parkordermgr.h"
#include "string.h"
#include <vector>
#include <memory>
#include "rpcstream.h"
#include "internalmessaging.h"
#include "stool.h"
#include "eventcentermodule.h"

using namespace std;

std::shared_ptr<ParkedOrderMgr> g_parkOrderMgrPtr = nullptr;

std::shared_ptr<ParkedOrderMgr> GetParkedOrderMgr()
{
    if(g_parkOrderMgrPtr)
    {
        return g_parkOrderMgrPtr;
    }
    g_parkOrderMgrPtr =  std::make_shared<ParkedOrderMgr>();
    return g_parkOrderMgrPtr;
}


ParkCondOrderMgrByUser::ParkCondOrderMgrByUser(const tagOneTradeUser &user)
{
    memset(&m_user, 0, sizeof(m_user));
    memcpy(&m_user, &user, sizeof(m_user));
    m_reqCount = 0;
}

void ParkCondOrderMgrByUser::cancelParkOrder(string parkId)
{
    auto search = m_parkOrderInfo.find(parkId);
    if(search != m_parkOrderInfo.end() && search->second.ParkedStatus == XT_PAOS_NotSend)
    {
        tagXTParkedOrderActionField cancelOrder;
        memset(&cancelOrder, 0, sizeof(tagXTParkedOrderActionField));
        cancelOrder.ParkedType = XT_CC_ParkedOrder;
        cancelOrder.ActionFlag = XT_AF_Delete;
        strncpy(cancelOrder.ParkedOrderID, parkId.c_str(), parkId.size());

        memcpy(&cancelOrder.ExCode, &(search->second.ExCode), sizeof(search->second.ExCode));
        reqParkedOrderAction(&cancelOrder);
    }
}

void ParkCondOrderMgrByUser::cancelCondOrder(string parkId)
{
    auto search = m_condOrderInfo.find(parkId);
    if(search != m_condOrderInfo.end() && search->second.ParkedStatus == XT_PAOS_NotSend)
    {
        tagXTParkedOrderActionField cancelOrder;
        memset(&cancelOrder, 0, sizeof(tagXTParkedOrderActionField));
        cancelOrder.ParkedType = search->second.ParkedType;
        cancelOrder.ActionFlag = XT_AF_Delete;
        strncpy(cancelOrder.ParkedOrderID, parkId.c_str(), parkId.size());

        memcpy(&cancelOrder.ExCode, &(search->second.ExCode), sizeof(search->second.ExCode));
        reqParkedOrderAction(&cancelOrder);
    }
}

int ParkCondOrderMgrByUser::reqParkedOrderInsert(tagXTParkedOrderField *inputOrder)
{
    if(!inputOrder) return -1;
    char tmpBuf[4096]{0};
    RpcWrite rw(tmpBuf, 4095);
    rw.WriteInt32(2);
    rw.WriteSolidPtr((char*)inputOrder, sizeof(tagXTParkedOrderField));
    rw.WriteInt32(++m_reqCount);
    if(!rw.IsError())
    {
        return sendRequest(2, 21, tmpBuf, rw.GetOffset(), m_user.broker, m_user.user);
    }
}

int ParkCondOrderMgrByUser::reqParkedOrderAction(tagXTParkedOrderActionField *inputAction)
{
    if(!inputAction) return -1;
    char tmpBuf[4096]{0};
    RpcWrite rw(tmpBuf, 4095);
    rw.WriteInt32(2);
    rw.WriteSolidPtr((char*)inputAction, sizeof(tagXTParkedOrderField));
    rw.WriteInt32(++m_reqCount);
    if(!rw.IsError())
    {
        return sendRequest(2, 22, tmpBuf, rw.GetOffset(), m_user.broker, m_user.user);
    }
}

int ParkCondOrderMgrByUser::reqQryParkedOrder(tagXTQryParkedOrderField *inputQry)
{
    if(!inputQry) return -1;
    qDebug() << "Inquiry Parkorder info !" ;
    char tmpBuf[4096]{0};
    RpcWrite rw(tmpBuf, 4095);
    rw.WriteInt32(2);
    rw.WriteSolidPtr((char*)inputQry, sizeof(tagXTQryParkedOrderField));
    rw.WriteInt32(++m_reqCount);
    if(!rw.IsError())
    {
        return sendRequest(2, 20, tmpBuf, rw.GetOffset(), m_user.broker, m_user.user);
    }
}

int ParkCondOrderMgrByUser::onRspQryParkedOrder(tagXTParkedOrderField *inputOrder, tagXTRspInfoField *pRspInfo, int reqId, bool isLast)
{
    if(inputOrder)
    {
        qDebug() << "receive park order insert " << inputOrder->ParkedOrderID;
        if(pRspInfo)
        {
            qDebug()<<pRspInfo->ErrorMsg;
            return 0;
        }
        if(inputOrder->ParkedType == XT_CC_ParkedOrder)
        {
            string key(inputOrder->ParkedOrderID);
            m_parkOrderInfo[key] = *inputOrder;
        }
        if((inputOrder->ParkedType >= XT_CC_LastPriceGreaterThanStopPrice && inputOrder->ParkedType <= XT_CC_AskPriceGreaterThanStopPrice)
           || (inputOrder->ParkedType >= XT_CC_AskPriceGreaterEqualStopPrice && inputOrder->ParkedType <= XT_CC_BidPriceLesserEqualStopPrice))
        {
            string key(inputOrder->ParkedOrderID);
            m_condOrderInfo[key] = *inputOrder;
        }
    }
}

int ParkCondOrderMgrByUser::onRtnParkedOrder(tagXTParkedOrderField *inputOrder)
{
    if(inputOrder)
    {
        qDebug() << "receive park order resp " << inputOrder->ParkedOrderID;
        if(inputOrder->ParkedType == XT_CC_ParkedOrder)
        {
            string key(inputOrder->ParkedOrderID);
            m_parkOrderInfo[key] = *inputOrder;
            if(inputOrder->ParkedStatus == XT_PAOS_NotSend)
            {
                m_parkTime[key] = QTime::QTime::currentTime();
            }

            QEventCenter* pEventCenter = GetEventCenter();
            ParkOrderEvent* e = new ParkOrderEvent(CET_ParkOrder);
            e->usertype = 0;
            e->subevent = ORDER_EVENT_UPDATE;
            e->broker = inputOrder->BrokerID;
            strncpy(e->user, inputOrder->UserID, strlen(inputOrder->UserID));
            qDebug() << "New park order incoming , " << inputOrder->OrderRef;
            pEventCenter->PostParkOrderMessage(e);
            return 1;
        }
        if((inputOrder->ParkedType >= XT_CC_LastPriceGreaterThanStopPrice && inputOrder->ParkedType <= XT_CC_AskPriceGreaterThanStopPrice)
           || (inputOrder->ParkedType >= XT_CC_AskPriceGreaterEqualStopPrice && inputOrder->ParkedType <= XT_CC_BidPriceLesserEqualStopPrice))
        {
            string key(inputOrder->ParkedOrderID);
            m_condOrderInfo[key] = *inputOrder;
            if(inputOrder->ParkedStatus == XT_PAOS_NotSend)
            {
                m_parkTime[key] = QTime::QTime::currentTime();
            }

            QEventCenter* pEventCenter = GetEventCenter();
            ParkOrderEvent* e = new ParkOrderEvent(CET_ConditionOrder);
            e->usertype = 1;
            e->subevent = ORDER_EVENT_UPDATE;
            e->broker = inputOrder->BrokerID;
            strncpy(e->user, inputOrder->UserID, strlen(inputOrder->UserID));
            qDebug() << "New park order incoming , " << inputOrder->OrderRef;
            pEventCenter->PostParkOrderMessage(e);

            return 1;
        }
    }
    return 0;
}

std::vector<tagXTParkedOrderField> ParkCondOrderMgrByUser::getParkOrders()
{
    vector<tagXTParkedOrderField> result;
    for(auto p = m_parkOrderInfo.begin(); p!=m_parkOrderInfo.end();++p)
    {
        result.push_back(p->second);
    }
    return result;
}

std::vector<tagXTParkedOrderField> ParkCondOrderMgrByUser::getCondOrders()
{
    vector<tagXTParkedOrderField> result;
    for(auto& ele:m_condOrderInfo)
    {
        result.push_back(ele.second);
    }
    return result;
}

std::unordered_map<string, QTime> &ParkCondOrderMgrByUser::getParkTime()
{
    return m_parkTime;
}

int ParkCondOrderMgrByUser::sendRequest(unsigned char mainType, unsigned char childType, const char *inbuf, unsigned short insize, int broker, const char *userId)
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
        pProducer->sendMsg("420", tempbuf, sizeof(tagTDUserInfo) + sizeof(tagComm_FrameHead)  + pHead->size);
        return   sizeof(tagTDUserInfo) + sizeof(tagComm_FrameHead)  + pHead->size  ;
    }
    return  -1;
}

ParkedOrderMgr::ParkedOrderMgr()
{

}

int ParkedOrderMgr::initUsers()
{
    auto pConf = GetConfigModule();
    if(pConf)
    {
        vector<tagOneTradeUser> users;
        auto size = pConf->GetAllTradeUser(users);
        if(size > 0)
        {
            for(auto& ele:users)
            {
                char key[255]{0};
                sprintf(key, "%d_%s", ele.broker, ele.user);
                string strkey(key);
                shared_ptr<ParkCondOrderMgrByUser> user =  make_shared<ParkCondOrderMgrByUser>(ele);
                m_parkCondOrdersForUsers.insert({strkey, user});

                tagXTQryParkedOrderField qry;
                memset(&qry, 0, sizeof(tagXTQryParkedOrderField));
                qry.ParkedType = XT_CC_ParkedOrder;
                user->reqQryParkedOrder(&qry);

                tagXTQryParkedOrderField qry1;
                memset(&qry1, 0, sizeof(tagXTQryParkedOrderField));
                user->reqQryParkedOrder(&qry1);
            }
        }
        return 1;
    }
    else
    {
        return -1;
    }
}

shared_ptr<ParkCondOrderMgrByUser> ParkedOrderMgr::findMgrByUser(const string &strkey)
{
    auto search = m_parkCondOrdersForUsers.find(strkey);
    if(search == m_parkCondOrdersForUsers.end())
    {
        return nullptr;
    }
    else
    {
        return m_parkCondOrdersForUsers[strkey];
    }
}

std::vector<std::shared_ptr<ParkCondOrderMgrByUser> > ParkedOrderMgr::getAllUsers()
{
    vector<shared_ptr<ParkCondOrderMgrByUser>> result;
    for(auto& ele:m_parkCondOrdersForUsers)
    {
        result.push_back(ele.second);
    }
    return result;
}

bool ParkedOrderMgr::handleBuf(const char *pbuf, int buflen)
{
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
            tagXTParkedOrderField* pField = (tagXTParkedOrderField*)rr.ReadSolidPtr();
            tagXTRspInfoField* pRspInfo = (tagXTRspInfoField*)rr.ReadSolidPtr();
            int reqId = rr.ReadInt32();
            bool bLast = rr.ReadInt8()>0;
            if(rr.IsError())
            {
                printf("OnRspQryParkOrder failed\n");
            }
            else
            {
                auto strkey = stool::genUserId(userInfo.broker, userInfo.userid);
                auto parkUser = findMgrByUser(strkey);
                if(parkUser)
                {
                    parkUser->onRspQryParkedOrder(pField, pRspInfo, reqId, bLast);
                }
            }
        }
        break;

        case 23:
        {
            int argc = rr.ReadInt32();
            tagXTParkedOrderField* pField = (tagXTParkedOrderField*)rr.ReadSolidPtr();
            if(rr.IsError())
            {
                printf("OnRtnParkOrder failed\n");
            }
            else
            {
                auto strkey = stool::genUserId(userInfo.broker, userInfo.userid);
                auto parkUser = findMgrByUser(strkey);
                if(parkUser)
                {
                    parkUser->onRtnParkedOrder(pField);
                }
            }
        }
    }
    return !rr.IsError();
}

