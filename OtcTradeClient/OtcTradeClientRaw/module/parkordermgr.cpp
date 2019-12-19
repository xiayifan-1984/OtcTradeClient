#include "parkordermgr.h"
#include "string.h"
#include "rpcstream.h"
#include "kafkamsgunit.h"
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


ParkCondOrderMgrByUser::ParkCondOrderMgrByUser(const tagTTradeUserParam &user)
{
    memset(&m_user, 0, sizeof(m_user));
    memcpy(&m_user, &user, sizeof(m_user));
    m_reqCount = 0;
}

//===============================================================================================================================================================
void ParkCondOrderMgrByUser::cancelParkOrder(string parkId)
{
    auto search = m_mapOrders.find(parkId);
    if(search != m_mapOrders.end() && search->second.ParkedStatus == XT_PAOS_NotSend)
    {
        tagXTParkedOrderActionField oField;
        memset(&oField, 0, sizeof(oField));
        oField.BrokerID = m_user.broker;
        strcpy(oField.UserID, m_user.user);
        oField.ExCode = search->second.ExCode;

        oField.ActionFlag = XT_AF_Delete;
        oField.ParkedType = search->second.ParkedType;
        strcpy(oField.ParkedOrderID, search->second.ParkedOrderID);

        reqParkedOrderAction(&oField);
    }
}

void  ParkCondOrderMgrByUser::manualSendOrder(std::string parkId)
{
    auto search = m_mapOrders.find(parkId);
    if(search != m_mapOrders.end() )
    {
        if( search->second.ParkedType == XT_PT_HUMAN && search->second.ParkedStatus == XT_PAOS_NotSend)
        {
            tagXTParkedOrderActionField oField;
            memset(&oField, 0, sizeof(oField));
            oField.BrokerID = m_user.broker;
            strcpy(oField.UserID, m_user.user);
            oField.ExCode = search->second.ExCode;

            oField.ActionFlag = XT_AF_HUMANSEND;
            oField.ParkedType = search->second.ParkedType;
            strcpy(oField.ParkedOrderID, search->second.ParkedOrderID);

            reqParkedOrderAction(&oField);
        }
    }
}

//===============================================================================================================================================================
int ParkCondOrderMgrByUser::reqParkedOrderInsert(tagXTParkedOrderInputField *pField)
{
    if(!pField)
    {
        return -1;
    }

    char tmpBuf[8192] = {0};
    RpcWrite rw(tmpBuf, 8191);
    rw.WriteInt32(2);
    rw.WriteSolidPtr((char*)pField, sizeof(tagXTParkedOrderInputField));
    rw.WriteInt32(++m_reqCount);
    if(!rw.IsError())
    {
        return sendRequest(2, 21, tmpBuf, rw.GetOffset() );
    }
}

int ParkCondOrderMgrByUser::reqParkedOrderAction(tagXTParkedOrderActionField *pField)
{
    if(!pField)
    {
        return -1;
    }

    char tmpBuf[8192] = {0};
    RpcWrite rw(tmpBuf, 8191);
    rw.WriteInt32(2);
    rw.WriteSolidPtr((char*)pField, sizeof(tagXTParkedOrderActionField));
    rw.WriteInt32(++m_reqCount);
    if(!rw.IsError())
    {
        return sendRequest(2, 22, tmpBuf, rw.GetOffset() );
    }
}

int ParkCondOrderMgrByUser::reqQryParkedOrder(tagXTQryParkedOrderField *pField)
{
    if(!pField)
    {
        return -1;
    }

    qDebug() << "Query ParkOrder info !" ;
    char tmpBuf[8192] = {0};
    RpcWrite rw(tmpBuf, 8191);
    rw.WriteInt32(2);
    rw.WriteSolidPtr((char*)pField, sizeof(tagXTQryParkedOrderField));
    rw.WriteInt32(++m_reqCount);
    if(!rw.IsError())
    {
        return sendRequest(2, 20, tmpBuf, rw.GetOffset() );
    }
}

int ParkCondOrderMgrByUser::onRspQryParkedOrder(tagXTParkedOrderField *pField, tagXTRspInfoField *pRspInfo, int reqId, bool isLast)
{
    if(pField)
    {
        qDebug() << "receive park order query " << pField->ParkedOrderID;

        string key(pField->ParkedOrderID);
        m_mapOrders[key] = *pField;
    }
}

int ParkCondOrderMgrByUser::onRtnParkedOrder(tagXTParkedOrderField *pField)
{
    if(pField)
    {
        qDebug() << "receive park order return " << pField->ParkedOrderID;

        string key(pField->ParkedOrderID);
        m_mapOrders[key] = *pField;

        QEventCenter* pEventCenter = GetEventCenter();
        ParkOrderEvent* e = new ParkOrderEvent(CET_ParkOrder);
        e->usertype = 0;
        e->subevent = ORDER_EVENT_UPDATE;
        e->broker = m_user.broker;
        strncpy(e->user, m_user.user, strlen(m_user.user));

        pEventCenter->PostParkOrderMessage(e);
        return 1;

    }
    return 0;
}

int         ParkCondOrderMgrByUser::getAllParkorder(vector<tagXTParkedOrderField>& arrOrder)
{
    auto itb = m_mapOrders.begin();
    while(itb != m_mapOrders.end() )
    {
        arrOrder.push_back(itb->second);
        itb++;
    }

    return arrOrder.size();
}

int ParkCondOrderMgrByUser::sendRequest(unsigned char mainType, unsigned char childType, const char *inbuf, unsigned short insize)
{
    char tempbuf[8192]={0};
    tagTDUserInfo* pUser = (tagTDUserInfo*)(tempbuf);
    pUser->broker = m_user.broker;
    strncpy(pUser->userid,  m_user.user, strlen(m_user.user));

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
        pProducer->sendMsg("422", tempbuf, sizeof(tagTDUserInfo) + sizeof(tagComm_FrameHead)  + pHead->size);
        return   sizeof(tagTDUserInfo) + sizeof(tagComm_FrameHead)  + pHead->size  ;
    }
    return  -1;
}


//...................................................................................................................................................................................
//...................................................................................................................................................................................
//...................................................................................................................................................................................
//...................................................................................................................................................................................
ParkedOrderMgr::ParkedOrderMgr()
{

}

int ParkedOrderMgr::initUsers()
{
    auto pConf = GetConfigModule();
    if(pConf)
    {
        vector<tagTTradeUserParam> users;
        auto size = pConf->GetAllTradeUser(users);
        if(size > 0)
        {
            for(auto& ele:users)
            {
                char key[255]{0};
                sprintf(key, "%d_%s", ele.broker, ele.user);
                string strkey(key);
                shared_ptr<ParkCondOrderMgrByUser> user =  make_shared<ParkCondOrderMgrByUser>(ele);
                m_mapUser2Ptr.insert({strkey, user});

                tagXTQryParkedOrderField    oField;
                memset(&oField, 0, sizeof(tagXTQryParkedOrderField));
                oField.BrokerID = ele.broker;
                strcpy(oField.UserID, ele.user);

                user->reqQryParkedOrder(&oField);

            }
        }
        return 1;
    }
    else
    {
        return -1;
    }
}

ParkCondOrderMgrByUser* ParkedOrderMgr::findMgrByUser(const string &strkey)
{
    auto search = m_mapUser2Ptr.find(strkey);
    if(search == m_mapUser2Ptr.end())
    {
        return nullptr;
    }
    else
    {
        return m_mapUser2Ptr[strkey].get();
    }
}

std::vector<std::shared_ptr<ParkCondOrderMgrByUser> > ParkedOrderMgr::getAllUsers()
{
    vector<shared_ptr<ParkCondOrderMgrByUser>> result;
    for(auto& ele:m_mapUser2Ptr)
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

