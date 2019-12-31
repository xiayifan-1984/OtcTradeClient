
#include "kafkamsgunit.h"

#include "windows.h"
#include "QDebug"
#include "qlibrary.h"
#include "otcoptionmodule.h"
#include "decodeinnermsg.h"
#include <algorithm>
#include "parkordermgr.h"
#include "twapordermgr.h"

//====================================================================================================================================================================
//====================================================================================================================================================================
//====================================================================================================================================================================
//====================================================================================================================================================================

CKafkaUnit* g_pkafkaUnit = nullptr;
CKafkaUnit* GetKafkaUnit()
{
    if(g_pkafkaUnit)
    {
        return g_pkafkaUnit;
    }
    g_pkafkaUnit =  new CKafkaUnit;
    return g_pkafkaUnit;
}


class IKafkaMsg
{
public:
    IKafkaMsg() {}
    //virtual ~IKafkaMsg() {}

    virtual void    initProducer(const char*) = 0;
    virtual void    initConsumer(const char *, const char *, fnConsumeCB, int, char**) = 0;
    virtual void    instantiateSenderReceiver() = 0;
    virtual void    sendMsg(const char*, char*, int) = 0;
    virtual void    startConsume() = 0;
    virtual void    stopConsume() = 0;
    virtual void    Release() = 0;
};

//====================================================================================================================================================================
//====================================================================================================================================================================
//====================================================================================================================================================================
//====================================================================================================================================================================

int     fnMsgHandler(const char* topic, int partition, long long offset, int buflen, const char* buf)
{
    qDebug() << "topic :" << topic << ",partition :" << partition;
    if(strcmp(topic, "411") == 0)
    {
        otcOptPositionRsp rsp;
        if(Decodeinnermsg::decodeOtcOptPositionRsp(buf, buflen, rsp))
        {
            qDebug() << "resId " << rsp.resID.c_str();
            qDebug() << "reqId " << rsp.reqID;
        }
        auto pOtcInfo = GetOtcOptionModule();
        std::for_each(rsp.positions.begin(), rsp.positions.end(), [pOtcInfo](otcOptPosition& pos)
        {
            pOtcInfo->addOtcPosition(pos);
        });
        return 1;
    }
    if(strcmp(topic, "423") == 0)
    {
        auto pPark = GetParkedOrderMgr();
        if(pPark)
        {
            pPark->handleBuf(buf, buflen);
        }
    }
    if(strcmp(topic, "431") == 0)
    {
        auto pTwapTrade = GetTwapOrderMgr();
        if(pTwapTrade)
        {
            pTwapTrade->handleBuf(buf, buflen);
        }
    }
    return 1;
}

CKafkaUnit::CKafkaUnit()
{
    m_impl = nullptr;
}

void CKafkaUnit::initProducer(const char *broker)
{
    if(!m_impl) return ;
     m_impl->initProducer(broker);
}

void CKafkaUnit::Instance()
{
    typedef IKafkaMsg* (__stdcall *pout)();

    QLibrary mylib("kafkaMsgHandler.dll");
    if(mylib.load())
    {
        pout pCreator = (pout)mylib.resolve("CreateInternalSenderReceiver");
        if(pCreator)
        {
           m_impl = (IKafkaMsg*)(pCreator());
        }
    }

    if(!m_impl)
    {
        qDebug()<<"load kafkamsghandler not valid ";
    }
    else
    {
        m_impl->instantiateSenderReceiver();
        qDebug() <<"load kafkamsghandler valid";
    }
}

void CKafkaUnit::sendMsg(const char *broker, char *buf, int len)
{
    if(!m_impl) return;
    m_impl->sendMsg(broker, buf, len);
}

void CKafkaUnit::startConsume()
{
    if(!m_impl) return;
    m_impl->startConsume();
}

void CKafkaUnit::stopConsume()
{
    if(!m_impl) return;
    m_impl->stopConsume();
}

void CKafkaUnit::Release()
{
    if(!m_impl) return;
    m_impl->Release();
}

void CKafkaUnit::initConsumer(const char *broker, const char *group, fnConsumeCB pfnCB, int topicCount, char *argv[])
{
    if(!m_impl) return;
    m_impl->initConsumer(broker, group, pfnCB, topicCount, argv);
}






