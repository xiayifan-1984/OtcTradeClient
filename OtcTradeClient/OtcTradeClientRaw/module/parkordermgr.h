#ifndef PARKORDERMGR_H
#define PARKORDERMGR_H
#include "XTTradestruct.h"
#include <string>
#include <unordered_map>
#include "codeinnermsg.h"
#include <memory>
#include "configmodule.h"
#include <vector>

class ParkOrderMgrByUser
{
public:
    ParkOrderMgrByUser(const tagOneTradeUser& user);
    ~ParkOrderMgrByUser(){}

    int reqParkedOrderInsert(tagXTParkedOrderField* inputOrder);
    int reqParkedOrderAction(tagXTParkedOrderActionField* inputAction);
    int reqQryParkedOrder(tagXTQryParkedOrderField* inputQry);

    int onRspQryParkedOrder(tagXTParkedOrderField* inputOrder, tagXTRspInfoField* pRspInfo, int reqId, bool isLast);
    int onRtnParkedOrder(tagXTParkedOrderField* inputOrder);

    std::vector<tagXTParkedOrderField> getParkOrders();
    std::vector<tagXTParkedOrderField> getCondOrders();
private:
    int sendRequest(unsigned char mainType, unsigned char childType, const char* inbuf, unsigned short  insize, int broker, const char* userId);
    tagOneTradeUser m_user;
    int m_reqCount;
    std::unordered_map<std::string, tagXTParkedOrderField> m_parkOrderInfo;
    std::unordered_map<std::string, tagXTParkedOrderField> m_condOrderInfo;
};

class ParkedOrderMgr
{
public:
    ParkedOrderMgr();
    ~ParkedOrderMgr(){}

    int initUsers();
    std::shared_ptr<ParkOrderMgrByUser> findMgrByUser(const string& strkey);
    bool handleBuf(const char* pbuf, int buflen);
private:
    std::unordered_map<std::string, std::shared_ptr<ParkOrderMgrByUser>> m_parkCondOrdersForUsers;
};

extern std::shared_ptr<ParkedOrderMgr> GetParkedOrderMgr();

#endif // PARKORDERMGR_H
