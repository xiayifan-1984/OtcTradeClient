
#ifndef PARKORDERDATA_H
#define PARKORDERDATA_H

#include <QtWidgets>
#include <memory>
#include <unordered_map>
#include <string>
#include "XTTParkedStruct.h"

class ParkOrderEvent;
class ParkOrderTable;
class CParkOrderData : public QWidget
{
    Q_OBJECT
public:
    explicit CParkOrderData(QWidget* parent = nullptr);
    ~CParkOrderData(){}

protected:
    void    updateParkOrderInfo();

private slots:
    void    onDoubleClickedTableView(const QModelIndex&);
    void    onParkOrderEvent(ParkOrderEvent* pEvent);
    void    on_combox_indexChange(int index);

private:
    void                    initControls();
    QComboBox*              _userSel;
    QTabWidget*             _tabViews;

    ParkOrderTable*         _parkOrderTab;

};



class ParkCondOrderMgrByUser;
class QOrderMgr;
class ParkOrderTable : public QTableView
{
    Q_OBJECT
public:
    explicit ParkOrderTable(QWidget* parent = nullptr);
    ~ParkOrderTable(){}

public:
    void    setUserMgr(ParkCondOrderMgrByUser* parkUserMgr);
    void    manualSendParkOrder(ParkCondOrderMgrByUser* parkUserMgr, int row);
    void    cancelParkOrder(ParkCondOrderMgrByUser* parkUserMgr, int row);
    void    cancelAllParkOrder(ParkCondOrderMgrByUser* parkUserMgr);

private:
    void    initControls();
    void    setParkHeader();
    void    fillRow(int row, tagXTParkedOrderField& order);

private:
    QStandardItemModel* _parkOrderModel;
    std::unordered_map<int, std::pair<std::string, char>> m_rowIdx2ParkId;

};








#endif

