#ifndef PARKORDERMGR_H
#define PARKORDERMGR_H
#include "XTTradestruct.h"
#include <string>
#include <unordered_map>
#include "codeinnermsg.h"
#include <memory>
#include "configmodule.h"
#include <vector>

class ParkCondOrderMgrByUser
{
public:
    ParkCondOrderMgrByUser(const tagOneTradeUser& user);
    ~ParkCondOrderMgrByUser(){}

    void cancelParkOrder(std::string parkId);
    void cancelCondOrder(std::string parkId);
    int reqParkedOrderInsert(tagXTParkedOrderField* inputOrder);
    int reqParkedOrderAction(tagXTParkedOrderActionField* inputAction);
    int reqQryParkedOrder(tagXTQryParkedOrderField* inputQry);

    int onRspQryParkedOrder(tagXTParkedOrderField* inputOrder, tagXTRspInfoField* pRspInfo, int reqId, bool isLast);
    int onRtnParkedOrder(tagXTParkedOrderField* inputOrder);

    std::vector<tagXTParkedOrderField> getParkOrders();
    std::vector<tagXTParkedOrderField> getCondOrders();
    std::unordered_map<std::string, QTime>& getParkTime();
private:
    int sendRequest(unsigned char mainType, unsigned char childType, const char* inbuf, unsigned short  insize, int broker, const char* userId);
    tagOneTradeUser m_user;
    int m_reqCount;
    std::unordered_map<std::string, tagXTParkedOrderField> m_parkOrderInfo;
    std::unordered_map<std::string, tagXTParkedOrderField> m_condOrderInfo;

    std::unordered_map<std::string, QTime> m_parkTime;
};

class ParkedOrderMgr
{
public:
    ParkedOrderMgr();
    ~ParkedOrderMgr(){}

    int initUsers();
    std::shared_ptr<ParkCondOrderMgrByUser> findMgrByUser(const string& strkey);
    std::vector<std::shared_ptr<ParkCondOrderMgrByUser>> getAllUsers();
    bool handleBuf(const char* pbuf, int buflen);
private:
    std::unordered_map<std::string, std::shared_ptr<ParkCondOrderMgrByUser>> m_parkCondOrdersForUsers;
};

extern std::shared_ptr<ParkedOrderMgr> GetParkedOrderMgr();

#endif // PARKORDERMGR_H
