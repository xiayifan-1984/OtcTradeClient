#ifndef SDIORDERFRAME_DEFINE_H
#define SDIORDERFRAME_DEFINE_H

#include <QtWidgets>

class OrderEventArgs;
class QOrderMgr;
class ModelSingleOrder;
class CDockTitlebar;
class CTwapSingleData;
class COrderSingleData;
class CParkSingleData;
class CSDIOrderFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CSDIOrderFrame(QWidget *parent = nullptr);
    ~CSDIOrderFrame();

public:
    void                setUserAccount(int usertype, int broker, char* userid);
    void                setUserAccount(QOrderMgr* p);
    void                initControls();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                onTabSelectChanged(int index);
    void                onTitleButtonClick(int buttonID);

private:
    QTabWidget*         _tabWidget;

    //挂单视图
    COrderSingleData*   _acceptWidget;
    //所有委托单视图
    COrderSingleData*   _allorderWidget;
    //Twap视图
    CTwapSingleData*     _twapWidget;
    //Park视图
    CParkSingleData*     _parkWidget;


    QOrderMgr*          _orderMrg;

private:
    CDockTitlebar*      _titlebar;

};

















#endif
