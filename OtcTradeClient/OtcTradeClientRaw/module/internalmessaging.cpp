#include "internalmessaging.h"
#include "windows.h"
#include "QDebug"
#include "qlibrary.h"
#include "./util/decodeinnermsg.h"
#include "otcoptionmodule.h"
#include "decodeinnermsg.h"
#include <algorithm>
#include "parkordermgr.h"
#include "algoordermgr.h"

InternalSenderReceiver* g_internalSenderReceiver = nullptr;
InternalSenderReceiver* GetInternalMsgSenderReceiver()
{
    if(g_internalSenderReceiver)
    {
        return g_internalSenderReceiver;
    }
    g_internalSenderReceiver =  new InternalSenderReceiver;
    return g_internalSenderReceiver;
}

int internalMsgHandler(const char* topic, int partition, long long offset, int buflen, const char* buf)
{
    qDebug() << "topic " << topic << ", partition " << partition;
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
    if(strcmp(topic, "421") == 0)
    {
        auto pPark = GetParkedOrderMgr();
        if(pPark)
        {
            pPark->handleBuf(buf, buflen);
        }
    }
    if(strcmp(topic, "431") == 0)
    {
        auto pAlgoTrade = GetAlgoOrderMgr();
        if(pAlgoTrade)
        {
            pAlgoTrade->handleBuf(buf, buflen);
        }
    }
    return 1;
}

InternalSenderReceiver::InternalSenderReceiver()
{
    m_impl = nullptr;
}

void InternalSenderReceiver::initProducer(const char *broker)
{
    if(!m_impl) return ;
     m_impl->initProducer(broker);
}

void InternalSenderReceiver::instantiateSenderReceiver()
{
   // HMODULE hDll = LoadLibraryA("kafkaMsgHandler.dll");
   // if(!hDll) return;
    typedef IInternalSenderReceiver* (__stdcall *pout)();

   // pCreateSenderReceiver pCreator = (pCreateSenderReceiver)::GetProcAddress(hDll, "CreateInternalSenderReceiver");
   // if(pCreator)
   // {
   //     m_impl = pCreator();
   // }
    // m_impl = CreateInternalSenderReceiver();
   // typedef void (__stdcall *pout)();
    //QLibrary mylib("MyDLL.dll");
    QLibrary mylib("kafkaMsgHandler.dll");
    if(mylib.load())
    {
        //pout pCreator = (pout)mylib.resolve("EndWork");
        pout pCreator = (pout)mylib.resolve("CreateInternalSenderReceiver");
        if(pCreator)
        {
           m_impl = (IInternalSenderReceiver*)(pCreator());
        }
    }
    if(!m_impl)
    {
        qDebug()<<"not valid ";
    }
    else
    {
       qDebug() <<"valid";
    }
}

void InternalSenderReceiver::sendMsg(const char *broker, char *buf, int len)
{
    if(!m_impl) return;
    m_impl->sendMsg(broker, buf, len);
}

void InternalSenderReceiver::startConsume()
{
    if(!m_impl) return;
    m_impl->startConsume();
}

void InternalSenderReceiver::stopConsume()
{
    if(!m_impl) return;
    m_impl->stopConsume();
}

void InternalSenderReceiver::Release()
{
    if(!m_impl) return;
    m_impl->Release();
}

void InternalSenderReceiver::initConsumer(const char *broker, const char *group, fnConsumeCB pfnCB, int topicCount, char *argv[])
{
    if(!m_impl) return;
    m_impl->initConsumer(broker, group, pfnCB, topicCount, argv);
}



