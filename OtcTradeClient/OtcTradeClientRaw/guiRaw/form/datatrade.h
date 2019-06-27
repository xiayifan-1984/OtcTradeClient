#ifndef DATATRADEDEFINE_H
#define DATATRADEDEFINE_H

#include <QtWidgets>

class TransactionEventArgs;
class ModelSingleTrade;
class MultiTradeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MultiTradeWidget(QWidget *parent = nullptr);
    ~MultiTradeWidget();

protected:
    void                initControls();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                on_combox_indexChange(int index);
    void                onTransactEvent(TransactionEventArgs* e);

private:
    QLabel*             _label;
    QComboBox*          _combox;
    QTableView*         _tableview;
    ModelSingleTrade*   _customModel;

};

//单账户
class QOrderMgr;
class SingleTradeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SingleTradeWidget(QWidget *parent = nullptr);
    ~SingleTradeWidget();

public:
    void                setUserAccount(int usertype, int broker, char* userid);

protected:
    void                initControls();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                onTransactEvent(TransactionEventArgs* e);

private:
    QTableView*         _tableview;
    ModelSingleTrade*   _customModel;
    QOrderMgr*          _orderMrg;

};







#endif
