#include "modeldatatwap.h"
#include "trademodule.h"
#include "twapordermgr.h"
#include "stool.h"

extern bool OrderLower(const tagXTOrderField& a, const tagXTOrderField& b);

ModelTwapOrder::ModelTwapOrder(QObject *parent/* = nullptr */):
    ModelSingleOrder (parent)
{

}

ModelTwapOrder::~ModelTwapOrder()
{

}

void        ModelTwapOrder::setTwapMgr(std::string twapRef, QOrderMgr* p)
{
    beginResetModel();

    release();

    if(p)
    {
        auto pTwapCtx = GetTwapOrderMgr();
        int usertype=0;
        int broker =0;
        char szuser[32]={0};

        p->WhoIs(usertype, broker, szuser);
        auto twapUser = stool::genUserId(broker, szuser);
        auto user = pTwapCtx->findOrderMgr(twapUser);
        if(!user) return;

        const auto& twapRef2Orders = user->getTwapRef2OrderRefMap();
        auto search = twapRef2Orders.find(twapRef);
        if(search == twapRef2Orders.end()) return;

        const auto& ordersRecord = search->second;

        int ncount = p->GetAllOrder(nullptr, 0);
        if (ncount > 0)
        {
            tagXTOrderField* pArr = new tagXTOrderField[ncount];
            p->GetAllOrder(pArr, ncount);

            for (int i = 0; i < ncount; i++)
            {
                //[001]加入
                string orderRef = pArr[i].OrderRef;
                auto search = find_if(ordersRecord.begin(), ordersRecord.end(), [&](auto& ref){
                    return ref == orderRef;
                });
                if(search == ordersRecord.end()) continue;
                m_arrOrderTable.push_back(pArr[i]);
            }

            //[004]排序
            std::sort(m_arrOrderTable.begin(), m_arrOrderTable.end(), OrderLower);
            delete[] pArr;
        }
    }
    endResetModel();
}

