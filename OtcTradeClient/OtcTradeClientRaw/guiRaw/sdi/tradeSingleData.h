
#ifndef TRADESINGLEDATA_DEFINE_H
#define TRADESINGLEDATA_DEFINE_H

#include <QtWidgets>

class TransactionEventArgs;
class QOrderMgr;
class ModelSingleTrade;
class CTradeSingleData : public QWidget
{
    Q_OBJECT

public:
    explicit CTradeSingleData(QWidget* parent = nullptr);
    virtual ~CTradeSingleData(){}

public:
    void                setUserAccount(int usertype, int broker, char* userid);
    void                setUserAccount(QOrderMgr* p);

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                onTransactEvent(TransactionEventArgs* e);

private:
    void                initControls();

    QTableView*         _tblViewNormal;
    ModelSingleTrade*   _modelNormal;
    QOrderMgr*          _orderMrg;

};


//成交汇总画面
class ModelTotalTrade;
class CTradeTotalData : public QWidget
{
    Q_OBJECT

public:
    explicit CTradeTotalData(QWidget* parent = nullptr);
    virtual ~CTradeTotalData(){}

public:
    void                setUserAccount(int usertype, int broker, char* userid);
    void                setUserAccount(QOrderMgr* p);

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                onTransactEvent(TransactionEventArgs* e);

private:
    void                initControls();

    QTableView*         _tblViewNormal;
    ModelTotalTrade*    _modelNormal;
    QOrderMgr*          _orderMrg;

};


























#endif

