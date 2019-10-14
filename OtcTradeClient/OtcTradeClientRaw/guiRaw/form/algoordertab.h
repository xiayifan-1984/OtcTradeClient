#ifndef ALGOORDERTAB_H
#define ALGOORDERTAB_H\

#include <QtWidgets>
#include <memory>
#include <algoordermgr.h>
#include "codeinnermsg.h"
#include <unordered_map>
#include <string>
#include "ordermgrimpl.h"

class AlgoOrderTab : public QTableView
{
    Q_OBJECT
public:
    explicit AlgoOrderTab(QWidget* parent = nullptr);
    ~AlgoOrderTab(){}

    void setUserMgr(AlgoOrderMgrByUser& algoUserMgr);
    std::string findParkId(int rowIdx);
    void showDetailOrders(QOrderMgr* orderMgr, int row, int col);
    void cancelTwapOrder(AlgoOrderMgrByUser& algoUserMgr, int row);

private:
    void initControls();
    void setAlgoHeader();
    QStandardItemModel* _algoOrderModel;
    void fillRow(int row, tagXTAlgoOrderField& order, const AlgoOrderMgrByUser* pAlgoUser);
    std::unordered_map<int, std::pair<std::string, XTAlgoOrderType>> m_rowIdx2AlgoId;
};
#endif // ALGOORDERTAB_H
