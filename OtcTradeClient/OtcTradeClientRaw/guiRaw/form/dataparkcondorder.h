#ifndef DATAPARKCONDORDER_H
#define DATAPARKCONDORDER_H

#include <QtWidgets>
#include "parkedordertab.h"
#include "condordertab.h"
#include "eventcentermodule.h"

class OrderEventArgs;
class DataParkCondOrder : public QWidget
{
    Q_OBJECT
public:
    explicit DataParkCondOrder(QObject* parent = nullptr);
    ~DataParkCondOrder();

    void updateParkOrderInfo(int orderType);
private slots:
    void onClickedTableView(const QModelIndex&);
    void onOrderEvent(ParkOrderEvent* pEvent);
    void on_combox_indexChange(int);

private:
    void    initControls();
    std::string getCurParkUser();
    QComboBox* _userSel;
    QTabWidget*   _tabViews;

    ParkedOrderTab* _parkOrderView;
    CondOrderTab*   _condOrderView;

};

#endif // DATAPARKCONDORDER_H
