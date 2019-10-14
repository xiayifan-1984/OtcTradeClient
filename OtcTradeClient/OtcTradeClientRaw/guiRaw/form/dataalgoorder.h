#ifndef DATAALGOORDER_H
#define DATAALGOORDER_H

#include <QtWidgets>
#include "parkedordertab.h"
#include "condordertab.h"
#include "eventcentermodule.h"
#include "algoordertab.h"

class OrderEventArgs;
class DataAlgoOrder : public QWidget
{
    Q_OBJECT
public:
    explicit DataAlgoOrder(QObject* parent = nullptr);
    ~DataAlgoOrder(){};

    void updateAlgoOrderInfo(XTAlgoOrderType algoType);
private slots:
    void onClickedTableView(const QModelIndex&);
    void onOrderEvent(AlgoOrderEvent* pEvent);
    void onInsertOrderEvent(OrderEventArgs* pEvent);
    void on_combox_indexChange(int index);

private:
    void    initControls();
    QComboBox* _userSel;
    QTabWidget*   _tabViews;

    AlgoOrderTab*          _algoOrderTab;

    AlgoOrderMgrByUser* getCurrentUser();
};



#endif // DATAALGOORDER_H
