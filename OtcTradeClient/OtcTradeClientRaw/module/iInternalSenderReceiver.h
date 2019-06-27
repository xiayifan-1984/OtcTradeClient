#pragma once

#include <functional>
//using fnConsumeCB = std::function<int(const char*, int, long long, int, const char*)>;
typedef int (*fnConsumeCB)(const char*, int, long long, int, const char*);

#pragma pack(1)
class IInternalSenderReceiver
{
public:
    IInternalSenderReceiver() {}
    //virtual ~IInternalSenderReceiver() {}
    virtual void initProducer(const char*) = 0;
    virtual void initConsumer(const char *, const char *, fnConsumeCB, int, char**) = 0;
    virtual void instantiateSenderReceiver() = 0;
    virtual void sendMsg(const char*, char*, int) = 0;
    virtual void startConsume() = 0;
    virtual void stopConsume() = 0;
    virtual void Release() = 0;
};
#pragma pack()
