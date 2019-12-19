#ifndef TWAPORDERDATA_H
#define TWAPORDERDATA_H

#include <QtWidgets>
#include <memory>
#include <unordered_map>
#include <string>
#include "XTTwapStruct.h"

class TwapOrderEvent;
class OrderEventArgs;
class TwapOrderTable;
class CTwapOrderData : public QWidget
{
    Q_OBJECT
public:
    explicit CTwapOrderData(QWidget* parent = nullptr);
    ~CTwapOrderData(){};

protected:
    void    updateTwapOrderInfo();

private slots:
    void    onDoubleClickedTableView(const QModelIndex&);
    void    onOrderEvent(TwapOrderEvent* pEvent);
    void    onInsertOrderEvent(OrderEventArgs* pEvent);
    void    on_combox_indexChange(int index);

private:
    void                    initControls();
    QComboBox*              _userSel;
    QTabWidget*             _tabViews;

    TwapOrderTable*           _twapOrderTab;

};



class TwapOrderMgrByUser;
class QOrderMgr;
class TwapOrderTable : public QTableView
{
    Q_OBJECT
public:
    explicit TwapOrderTable(QWidget* parent = nullptr);
    ~TwapOrderTable(){}

public:
    void    setUserMgr(TwapOrderMgrByUser* twapUserMgr);
    //std::string findParkId(int rowIdx);
    void    showDetailOrders(QOrderMgr* orderMgr, int row, int col);

    void    cancelTwapOrder(TwapOrderMgrByUser* twapUserMgr, int row);
    void    cancelAllTwapOrder(TwapOrderMgrByUser* twapUserMgr);

private:
    void    initControls();
    void    setTwapHeader();
    void    fillRow(int row, tagXTTwapOrderField& order, const TwapOrderMgrByUser* pTwapUser);

private:
    QStandardItemModel* _twapOrderModel;
    std::unordered_map<int, std::pair<std::string, char>> m_rowIdx2TwapId;

};







#endif // TWAPORDERDATA_H
