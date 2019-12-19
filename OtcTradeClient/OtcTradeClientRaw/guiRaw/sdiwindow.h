#ifndef CSDIWINDOW_H
#define CSDIWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class QOrderMgr;
class CSDIWindow : public QMainWindow
{
    Q_OBJECT

public:
    CSDIWindow(QWidget *parent = 0);
    ~CSDIWindow();

public:
    void            setSDIUser(const QVariant& var_orderMgr, const QString& strAlias);
    void            initControls();

protected:
    QWidget*        makeDockWidget_01();        //创建01画面: 资金
    QWidget*        makeDockWidget_02();        //创建02画面: 行情
    QWidget*        makeDockWidget_31();        //创建31画面: 智能下单板
    QWidget*        makeDockWidget_32();        //创建32画面: 委托
    QWidget*        makeDockWidget_33();        //创建33画面: 成交
    QWidget*        makeDockWidget_04();        //创建04画面: 持仓

protected:
    void            createActions();
    void            createStatusbar();

private slots:
    void            about();
    void            ShowMDIFrame();
    void            ShowSDIUser(QAction* pAction);
    void            ShowConfiguration();
    void            ShowExit();

private:
    QSplitter *     m_pSplitMain;
    QSplitter *     m_pSplitSub2;
    QOrderMgr*      m_pOrderMgr;

};

#endif // CSDIWINDOW_H
