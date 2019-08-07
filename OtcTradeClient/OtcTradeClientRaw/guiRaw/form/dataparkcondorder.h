#ifndef DATAPARKCONDORDER_H
#define DATAPARKCONDORDER_H

#include <QtWidgets>
#include "parkedordertab.h"
#include "condordertab.h"

class DataParkCondOrder : public QWidget
{
    Q_OBJECT
public:
    explicit DataParkCondOrder(QObject* parent = nullptr);
    ~DataParkCondOrder();
/*
private slots:
    void onParkOrderMessage();
*/
private:
    void    initControls();
    QComboBox* _userSel;
    QTabWidget*   _tabViews;

    ParkedOrderTab* _parkOrderView;
    CondOrderTab*   _condOrderView;

};

#endif // DATAPARKCONDORDER_H
