#ifndef TWAPORDERMGR_H
#define TWAPORDERMGR_H

#include <string>
#include <map>
#include "codeinnermsg.h"
#include <vector>
#include <set>
#include "configmodule.h"
#include <utility>
#include <condition_variable>
#include <mutex>
#include <QString>
#include <memory>

class TwapOrderMgrByUser;
using TwapOrderRef = std::string;
using TwapUserRef = std::string;

using TwapRelationOrders = std::map<TwapOrderRef, std::set<std::string>>;

class TwapOrderMgrByUser
{
public:
    TwapOrderMgrByUser(const tagTTradeUserParam& user);

public:
    int         DoReqTwapOrderInsert(tagXTTwapInputOrderField* input);
    int         DoReqTwapOrderAction(const string& twapid, char twapType);
    int         DoReqQryTwapOrder(tagXTTwapQryOrderField* input);
    int         OnRspQryTwapOrder(tagXTTwapOrderField* input, tagXTRspInfoField* pRspInf, int reqId, bool isLast);
    int         OnTwapOrderRtn(tagXTTwapOrderField* input);
    std::vector<std::pair<TwapOrderRef, tagXTTwapOrderField>> getAllTwapOrders();

    int         OnRspQryTwapOrderDetail(tagXTTwapOrderInsertRtnField* input, tagXTRspInfoField* pRspInf, int reqId, bool isLast);
    int         OnTwapOrderInsertRtn(tagXTTwapOrderInsertRtnField* input);

    QString     getErrorMsg(const string& orderRef, char twaptype) const;
    TwapRelationOrders& getTwapRef2OrderRefMap();
    int         tradedOrders(const std::string& twapid) const;

protected:
    int         sendRequest(unsigned char mainType, unsigned char childType, const char* inbuf, unsigned short  insize, int broker, const char* userId);
    void        updateOrderInfo(tagXTTwapInputOrderField* input);
    int         cancelTwapOrder(tagXTTwapOrderActionField* input);

private:
    std::map<char, std::map<TwapOrderRef, tagXTTwapOrderField>>        m_twapOrderCtx;     //twaptype ====> map { twapid ===> tagXTTwapOrderField  }
    TwapRelationOrders      m_relativeOrdersRecord;     // twapid ===>  vector<OrderRef>

    tagTTradeUserParam      m_user;
    int                     m_reqCount;

};

class TwapOrderMgr
{
public:
    TwapOrderMgr() = default;
    ~TwapOrderMgr() = default;

public:    
    int         initUsers();
    bool        handleBuf(const char* pbuf, int buflen);

    TwapOrderMgrByUser*     findOrderMgr(const std::string& userRef);

private:
    std::map<TwapUserRef, std::shared_ptr<TwapOrderMgrByUser> >        m_mapUser2Ptr;
    mutex                   mut;
    condition_variable      cond;
};

extern TwapOrderMgr*        GetTwapOrderMgr();

#endif // TWAPORDERMGR_H
