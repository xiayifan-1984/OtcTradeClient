#include "parkordermgr.h"
#include "string.h"
#include <vector>
#include <memory>
#include "rpcstream.h"
#include "internalmessaging.h"
#include "stool.h"

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


ParkOrderMgrByUser::ParkOrderMgrByUser(const tagOneTradeUser &user)
{
    memset(&m_user, 0, sizeof(m_user));
    memcpy(&m_user, &user, sizeof(m_user));
    m_reqCount = 0;
}

int ParkOrderMgrByUser::reqParkedOrderInsert(tagXTParkedOrderField *inputOrder)
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

int ParkOrderMgrByUser::reqQryParkedOrder(tagXTQryParkedOrderField *inputQry)
{
    if(!inputQry) return -1;
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

int ParkOrderMgrByUser::onRspQryParkedOrder(tagXTParkedOrderField *inputOrder, tagXTRspInfoField *pRspInfo, int reqId, bool isLast)
{
    if(inputOrder)
    {
        qDebug() << "receive park order insert " << inputOrder->ParkedOrderID;
    }
}

int ParkOrderMgrByUser::onRtnParkedOrder(tagXTParkedOrderField *inputOrder)
{
    if(inputOrder)
    {
        qDebug() << "receive park order resp " << inputOrder->ParkedOrderID;
    }
}

std::vector<tagXTParkedOrderField> ParkOrderMgrByUser::getParkOrders()
{
    vector<tagXTParkedOrderField> result;
    for(auto& ele:m_parkOrderInfo)
    {
        result.push_back(ele.second);
    }
    return result;
}

std::vector<tagXTParkedOrderField> ParkOrderMgrByUser::getCondOrders()
{
    vector<tagXTParkedOrderField> result;
    for(auto& ele:m_condOrderInfo)
    {
        result.push_back(ele.second);
    }
    return result;
}

int ParkOrderMgrByUser::sendRequest(unsigned char mainType, unsigned char childType, const char *inbuf, unsigned short insize, int broker, const char *userId)
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
                m_parkCondOrdersForUsers.insert({strkey, make_shared<ParkOrderMgrByUser>(ele)});
            }
        }
        return 1;
    }
    else
    {
        return -1;
    }
}

shared_ptr<ParkOrderMgrByUser> ParkedOrderMgr::findMgrByUser(const string &strkey)
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

bool ParkedOrderMgr::handleBuf(const char *pbuf, int buflen)
{
    tagTDUserInfo userInfo;
    memcpy(&userInfo, pbuf, sizeof(tagTDUserInfo));

    buflen -= sizeof(tagTDUserInfo);

    if(buflen <= sizeof(tagComm_FrameHead))
    {
        return false;
    }

    tagComm_FrameHead* pHead = nullptr;
    memcpy(pHead, pbuf+sizeof(tagTDUserInfo), sizeof(tagComm_FrameHead));

    buflen -= sizeof(tagComm_FrameHead);

    if(buflen < pHead->size)
    {
        return false;
    }

    RpcRead rr;
    rr.BindStream((const_cast<char*>(pbuf))+sizeof(tagComm_FrameHead)+sizeof(tagTDUserInfo), buflen);
    switch(pHead->childType)
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
                auto strkey = stool::genParkUserId(userInfo.broker, userInfo.userid);
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
                auto strkey = stool::genParkUserId(userInfo.broker, userInfo.userid);
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

