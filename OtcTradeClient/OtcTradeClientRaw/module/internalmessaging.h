#ifndef INTERNALMESSAGING_H
#define INTERNALMESSAGING_H

#include <functional>
#include "iInternalSenderReceiver.h"

class InternalSenderReceiver
{
 public:
    InternalSenderReceiver();
    void initProducer(const char*);
    void initConsumer(const char *, const char *, fnConsumeCB , int , char**);
    void instantiateSenderReceiver();
    void sendMsg(const char*, char*, int);
    void startConsume();
    void stopConsume();
    void Release();
 private:
    IInternalSenderReceiver* m_impl;
};

extern InternalSenderReceiver* GetInternalMsgSenderReceiver();
extern int internalMsgHandler(const char* topic, int partition, long long offset, int buflen, const char* buf);
#endif // INTERNALMESSAGING_H
