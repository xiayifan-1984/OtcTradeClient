#ifndef DATAPOSITIONDEFINE_H
#define DATAPOSITIONDEFINE_H

#include <QtWidgets>

class TransactionEventArgs;
class ModelNormalPosition;
class ModelCompositePosition;
class MultiPositionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MultiPositionWidget(QWidget *parent = nullptr);
    ~MultiPositionWidget();

protected:
    void                initControls();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                on_combox_indexChange(int index);
    void                onTransactEvent(TransactionEventArgs* e);

private:
    QLabel*                     _label;
    QComboBox*                  _combox;

    QTabWidget*                 _tabWidget;

    QTableView*                 _tblNormalView;
    ModelNormalPosition*        _csmNormalModel;

    QTableView*                 _tblCompositeView;
    ModelCompositePosition*     _csmCompositeModel;

};

class QOrderMgr;
class SinglePositionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SinglePositionWidget(QWidget *parent = nullptr);
    ~SinglePositionWidget();

public:
    void                setUserAccount(int usertype, int broker, char* userid);

protected:
    void                initControls();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                onTransactEvent(TransactionEventArgs* e);

private:

    QTabWidget*                 _tabWidget;

    QTableView*                 _tblNormalView;
    ModelNormalPosition*        _csmNormalModel;

    QTableView*                 _tblCompositeView;
    ModelCompositePosition*     _csmCompositeModel;

    QOrderMgr*                  _orderMrg;
};









#endif
