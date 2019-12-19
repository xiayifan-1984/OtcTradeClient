
#ifndef ORDERSINGLEDATA_H
#define ORDERSINGLEDATA_H

#include <QtWidgets>

class QOrderMgr;
class TwapOrderEvent;
class OrderEventArgs;
class TwapOrderTable;

//Twap
class CTwapSingleData : public QWidget
{
    Q_OBJECT
public:
    explicit CTwapSingleData(QWidget* parent = nullptr);
    virtual ~CTwapSingleData(){}

public:
    void                setUserAccount(int usertype, int broker, char* userid);
    void                setUserAccount(QOrderMgr* p);

    void                cancelAllOrder();
    void                cancelOneOrder();

protected:
    void                updateTwapOrderInfo();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                onDoubleClickedTableView(const QModelIndex&);
    void                onOrderEvent(TwapOrderEvent* pEvent);
    void                onInsertOrderEvent(OrderEventArgs* pEvent);

private:
    void                initControls();

    TwapOrderTable*     _twapOrderTab;
    QOrderMgr*          _orderMrg;

};


class ModelSingleOrder;
//普通委托，有2种模式(所有委托 / 仅挂单)
class COrderSingleData : public QWidget
{
    Q_OBJECT

public:
    explicit COrderSingleData(QWidget* parent = nullptr);
    virtual ~COrderSingleData(){}

public:
    void                setUserAccount(int usertype, int broker, char* userid);
    void                setUserAccount(QOrderMgr* p);
    void                initControls(int mode = 0);     //0 代表所有委托单  1代表挂单

    void                cancelAllOrder();
    void                cancelOneOrder();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    
private slots:
    void                onOrderEvent(OrderEventArgs*);
    void                on_tableview_doubleClicked(const QModelIndex&);

private:
    QTableView*         _tblViewNormal;
    ModelSingleOrder*   _modelNormal;
    QOrderMgr*          _orderMrg;
};


//Park
class ParkOrderTable;
class ParkOrderEvent;
class CParkSingleData : public QWidget
{
    Q_OBJECT
public:
    explicit CParkSingleData(QWidget* parent = nullptr);
    virtual ~CParkSingleData(){}

public:
    void                setUserAccount(int usertype, int broker, char* userid);
    void                setUserAccount(QOrderMgr* p);

    void                cancelAllOrder();
    void                cancelOneOrder();
    void                manualSendOneOrder();     //手工预埋， 发送

protected:
    void                updateParkOrderInfo();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                onDoubleClickedTableView(const QModelIndex&);
    void                onOrderEvent(ParkOrderEvent* pEvent);

private:
    void                initControls();

    ParkOrderTable*      _parkOrderTab;
    QOrderMgr*          _orderMrg;

};






#endif

