#ifndef ALGOORDERMGR_H
#define ALGOORDERMGR_H

#include <string>
#include <map>
#include "codeinnermsg.h"
#include <vector>
#include "configmodule.h"
#include <utility>
#include <condition_variable>
#include <mutex>
#include <QString>

class AlgoOrderMgrByUser;
using AlgoOrderRef = std::string;
using AlgoUserRef = std::string;
using AlgoOrderUserCtx = std::map<XTAlgoOrderType, std::map<AlgoOrderRef, tagXTAlgoOrderField>>;

using AlgoOrdersByUser = std::map<AlgoUserRef, AlgoOrderMgrByUser>;
using AlgoRelationOrders = std::map<AlgoOrderRef, std::vector<std::string>>;

class AlgoOrderMgrByUser
{
public:
    AlgoOrderMgrByUser(const tagOneTradeUser& user);

    int insertAlgoOrder(tagXTAlgoOrderField* input);
    int onAlgoOrderRtn(tagXTAlgoOrderField* input);
    int onAlgoRelatedOrderRtn(tagXTAlgoOrderInsertRtnField* input);
    int onAlgoOrderQryRtn(tagXTQryAlgoOrderField* input, tagXTRspInfoField* pRspInf, int reqId, bool isLast);
    int queryAlgoOrder(tagXTQryAlgoOrderField* input);

    QString getErrorMsg(const string& orderRef, XTAlgoOrderType ty) const;

    AlgoRelationOrders& getAlgoRef2OrderRefMap();
    int cancelOrder(const string& ref, XTAlgoOrderType twTy);
    std::vector<std::pair<AlgoOrderRef, tagXTAlgoOrderField>> getAlgoOrders();
    int tradedOrders(const tagXTAlgoOrderField& twOrder) const;
private:
    AlgoOrderUserCtx m_algoOrderCtx;
    AlgoRelationOrders m_relativeOrdersRecord;

    tagOneTradeUser m_user;
    int m_reqCount;
    int sendRequest(unsigned char mainType, unsigned char childType, const char* inbuf, unsigned short  insize, int broker, const char* userId);
    void updateOrderInfo(tagXTAlgoOrderField* input);
    int cancelAlgoOrder(tagXTAlgoOrderActionField* input);
};

class AlgoOrderMgr
{
public:
    AlgoOrderMgr() = default;
    ~AlgoOrderMgr() = default;
    int initUsers();
    bool handleBuf(const char* pbuf, int buflen);

    AlgoOrderMgrByUser* findOrderMgr(const std::string& userRef);
private:
    AlgoOrdersByUser m_algoOrderMgr;
    mutex mut;
    condition_variable cond;
};

extern AlgoOrderMgr* GetAlgoOrderMgr();

#endif // ALGOORDERMGR_H
