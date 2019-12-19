#ifndef SDIINPUTFRAME_DEFINE_H
#define SDIINPUTFRAME_DEFINE_H

#include <QtWidgets>

class QOrderMgr;
class CDockTitlebar;
class CSmartInputBox;
class CSmartTwapBox;
class CSmartParkBox;
class CSDIInputFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CSDIInputFrame(QWidget *parent = nullptr);
    virtual ~CSDIInputFrame();

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
    CSmartInputBox*     _kbInputbox;
    CSmartTwapBox*      _twapInputbox;
    CSmartParkBox*      _parkInputbox;

    QOrderMgr*          _orderMrg;

private:
    CDockTitlebar*      _titlebar;

};




#endif
