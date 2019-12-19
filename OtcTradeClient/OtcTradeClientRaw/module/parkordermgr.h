#ifndef PARKORDERMGR_H
#define PARKORDERMGR_H
#include "XTTParkedStruct.h"
#include <string>
#include <unordered_map>
#include "codeinnermsg.h"
#include <memory>
#include <vector>
#include "configmodule.h"

class ParkCondOrderMgrByUser
{
public:
    ParkCondOrderMgrByUser(const tagTTradeUserParam& user);
    ~ParkCondOrderMgrByUser(){}

public:
    int         getAllParkorder(vector<tagXTParkedOrderField>& arrOrder);

public:
    void        cancelParkOrder(std::string parkId);        //撤单
    void        manualSendOrder(std::string parkId);    //手工发单

public:
    int         reqParkedOrderInsert(tagXTParkedOrderInputField* pField);
    int         reqParkedOrderAction(tagXTParkedOrderActionField* pField);
    int         reqQryParkedOrder(tagXTQryParkedOrderField* pField);

    int         onRspQryParkedOrder(tagXTParkedOrderField* pField, tagXTRspInfoField* pRspInfo, int reqId, bool isLast);
    int         onRtnParkedOrder(tagXTParkedOrderField* pField);

private:
    int         sendRequest(unsigned char mainType, unsigned char childType, const char* inbuf, unsigned short  insize);

private:    
    tagTTradeUserParam      m_user;
    int                     m_reqCount;
    std::unordered_map<std::string, tagXTParkedOrderField>  m_mapOrders;

};

class ParkedOrderMgr
{
public:
    ParkedOrderMgr();
    ~ParkedOrderMgr(){}

public:
    int         initUsers();
    ParkCondOrderMgrByUser* findMgrByUser(const string& strkey);
    std::vector<std::shared_ptr<ParkCondOrderMgrByUser>> getAllUsers();
    bool        handleBuf(const char* pbuf, int buflen);

private:
    std::unordered_map<std::string, std::shared_ptr<ParkCondOrderMgrByUser>>    m_mapUser2Ptr;

};

extern std::shared_ptr<ParkedOrderMgr> GetParkedOrderMgr();

#endif // PARKORDERMGR_H
