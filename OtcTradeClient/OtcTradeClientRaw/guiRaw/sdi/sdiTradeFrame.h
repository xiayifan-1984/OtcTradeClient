#ifndef SDITRADEFRAME_DEFINE_H
#define SDITRADEFRAME_DEFINE_H

#include <QtWidgets>


class QOrderMgr;
class CTradeSingleData;
class CTradeTotalData;
class CDockTitlebar;
class CSDITradeFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CSDITradeFrame(QWidget *parent = nullptr);
    ~CSDITradeFrame();

public:
    void                setUserAccount(int usertype, int broker, char* userid);
    void                setUserAccount(QOrderMgr* p);
    void                initControls();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                onTabSelectChanged(int index);

private:
    QTabWidget*         _tabWidget;

    CTradeSingleData*   _tradeWidget;
    CTradeTotalData*    _totalWidget;

    QOrderMgr*          _orderMrg;

private:
    CDockTitlebar*      _titlebar;

};







#endif
