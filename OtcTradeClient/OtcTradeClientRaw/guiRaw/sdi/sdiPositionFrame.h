
#ifndef SDIPOSITIONFRAME_DEFINE_H
#define SDIPOSITIONFRAME_DEFINE_H

#include <QtWidgets>

class QOrderMgr;
class TransactionEventArgs;
class ModelNormalPosition;
class ModelCompositePosition;
class CDockTitlebar;
class CContractTableView;
class CLogTableView;
class CSDIPositionFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CSDIPositionFrame(QWidget* parent = nullptr);
    ~CSDIPositionFrame();

public:
    void        setUserAccount(int usertype, int broker, char*  userid);
    void        setUserAccount(QOrderMgr* p);
    void        initControls();

protected:
    void        closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void        onTransactEvent(TransactionEventArgs* e);
    void        onTabSelectChanged(int index);

private:
    QTabWidget*                 _tabWidget;

    QTableView*                 _tblViewNormal; //持仓信息
    ModelNormalPosition*        _modelNormalPosition;

    QTableView*                 _tblViewComposite; //组合持仓
    ModelCompositePosition*     _modelCompositePosition;

    CContractTableView*         _tblViewContract;
    CLogTableView*              _tblViewLog;

    QOrderMgr*                  _orderMrg;

private:
    CDockTitlebar*              _titlebar;


};









#endif
