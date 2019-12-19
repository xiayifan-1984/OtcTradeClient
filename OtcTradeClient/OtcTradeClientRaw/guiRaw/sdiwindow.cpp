#include "sdiwindow.h"

#include <vector>
#include <QDebug>

#include "docktitlebar.h"
#include "mainwindow.h"
#include "configmodule.h"
#include "XTCodec.h"
#include "trademodule.h"

#include "sdiPositionFrame.h"
#include "sdiTradeFrame.h"
#include "sdiOrderFrame.h"
#include "sdiCapitalFrame.h"
#include "sdiQuoteFrame.h"
#include "sdiInputFrame.h"



CSDIWindow::CSDIWindow(QWidget *parent)
    : QMainWindow(parent), m_pOrderMgr(nullptr)
{
    createActions();
    createStatusbar();

    setUnifiedTitleAndToolBarOnMac(true);

    setAttribute(Qt::WA_DeleteOnClose);
}

CSDIWindow::~CSDIWindow()
{

}

void CSDIWindow::createActions()
{
    QMenu *sysMenu = menuBar()->addMenu(tr("&System"));
    {
        QAction* pmdi = sysMenu->addAction(tr("MDI Frame"), this , &CSDIWindow::ShowMDIFrame );
        pmdi->setStatusTip(tr("显示一个MDI框架"));
        pmdi->setIcon(QIcon(":/image/400.bmp"));

        QMenu* psdi = sysMenu->addMenu(QIcon(":/image/406.bmp"), tr("SDI Frame") );
        std::vector<tagTTradeUserParam> arrUser;
        GetConfigModule()->GetAllTradeUser(arrUser);
        for(size_t i=0; i< arrUser.size(); i++ )
        {
            tagTTradeUserParam& o = arrUser[i];

            QOrderMgr* p = GetTradeModule()->GetMgr(o.type, o.broker, o.user);

            QString strName;
            std::string strmsg = o.aliasuser;
            strName = XTCodec::AfGbk_ToQString(strmsg);

            QVariant v = QVariant::fromValue((void *)p);

            QAction* psub = new QAction(strName, psdi);
            psub->setData(v);
            psdi->addAction(psub);

        }
        connect(psdi, SIGNAL(triggered(QAction*)), this, SLOT(ShowSDIUser(QAction*)) );

        QAction* new1= sysMenu->addAction(tr("Configuration"), this, &CSDIWindow::ShowConfiguration);
        new1->setStatusTip(tr("显示系统设置"));
        new1->setIcon(QIcon(":/image/001.bmp"));
        sysMenu->addSeparator();

        QAction* new2= sysMenu->addAction(tr("Close"), this, &CSDIWindow::ShowExit );
        new2->setIcon(QIcon(":/image/exit.bmp"));
        new2->setStatusTip(tr("关闭"));
    }

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    {
        QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &CSDIWindow::about);
        aboutAct->setStatusTip(tr("Show the application's About box"));

        QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
        aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    }

}

void CSDIWindow::createStatusbar()
{
    statusBar()->showMessage(tr("Ready"));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSDIWindow::about()
{
    QMessageBox::about(this, tr("About OtcTradeClient"),
                tr("The <b>OtcTradeClient</b> demonstrates a otc system, and how to trade and analysis future and option using Qt. "
                 ));
}

void CSDIWindow::ShowMDIFrame()
{
    CMainWindow* pnew = new CMainWindow();
    pnew->show();
}

void CSDIWindow::ShowSDIUser(QAction* pAction)
{
    CSDIWindow* pnew = new CSDIWindow();
    pnew->setSDIUser( pAction->data(), pAction->text() );
    pnew->initControls();
    pnew->showMaximized();
}

void CSDIWindow::ShowConfiguration()
{
    //@@@@@
}

void CSDIWindow::ShowExit()
{
    close();
}

void   CSDIWindow::setSDIUser(const QVariant& var_orderMgr, const QString& strAlias)
{
    QOrderMgr* _curMgr = (QOrderMgr *)var_orderMgr.value<void *>();
    m_pOrderMgr = _curMgr;

    QString strName =tr("OtcTradeClient(SDI) - ") + strAlias;
    setWindowTitle(strName);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void            CSDIWindow::initControls()
{
    //[1]创建横向的3个视图
    {
        m_pSplitSub2 = new QSplitter(Qt::Horizontal, nullptr);
        QWidget* m_vb0 = makeDockWidget_31();
        QWidget* m_vb1 = makeDockWidget_32();
        QWidget* m_vb2 = makeDockWidget_33();
        m_pSplitSub2->addWidget(m_vb0);
        m_pSplitSub2->addWidget(m_vb1);
        m_pSplitSub2->addWidget(m_vb2);

        //m_pSplitSub2->setStyleSheet("QSplitter::handle{background-color: red}");  //调试时，用来观察Splitter真实的大小

        //设置比例
        m_pSplitSub2->setStretchFactor(0, 1);
        m_pSplitSub2->setStretchFactor(1, 3);
        m_pSplitSub2->setStretchFactor(2, 3);
    }

    //[2]创建纵向的4个视图
    m_pSplitMain = new QSplitter(Qt::Vertical, nullptr);
    QWidget* m_hb0 = makeDockWidget_01();
    QWidget* m_hb1 = makeDockWidget_02();
    QWidget* m_hb4 = makeDockWidget_04();

    m_pSplitMain->addWidget(m_hb0);
    m_pSplitMain->addWidget(m_hb1);
    m_pSplitMain->addWidget(m_pSplitSub2);
    m_pSplitMain->addWidget(m_hb4);

    //m_pSplitMain->setStyleSheet("QSplitter::handle{background-color: blue}");   //调试时，用来观察Splitter真实的大小

    //[3]设置比例
    m_pSplitMain->setStretchFactor(0, 1);
    m_pSplitMain->setStretchFactor(1, 3);
    m_pSplitMain->setStretchFactor(2, 6);
    m_pSplitMain->setStretchFactor(3, 3);


    setCentralWidget(m_pSplitMain);
}

QWidget*   CSDIWindow::makeDockWidget_01()
{
    CSDICapitalFrame* pContainer = new CSDICapitalFrame();
    pContainer->setStyleSheet("border:1px solid rgb(145,145,145)");
    pContainer->initControls();
    pContainer->setUserAccount(m_pOrderMgr);
    pContainer->setMinimumSize(120, 80);

    return pContainer;
}

QWidget*   CSDIWindow::makeDockWidget_02()
{
    CSDIQuoteFrame* pContainer = new CSDIQuoteFrame();
    pContainer->setStyleSheet("border:1px solid rgb(145,145,145)");
    pContainer->initControls();
    pContainer->setUserAccount(m_pOrderMgr);
    return pContainer;
}

QWidget*    CSDIWindow::makeDockWidget_04()
{
    CSDIPositionFrame* pContainer = new CSDIPositionFrame();
    pContainer->setStyleSheet("border:1px solid rgb(145,145,145)");
    pContainer->initControls();
    pContainer->setUserAccount(m_pOrderMgr);

    return pContainer;
}

QWidget*    CSDIWindow::makeDockWidget_31()
{
    CSDIInputFrame* pContainer = new CSDIInputFrame();
    pContainer->setStyleSheet("border:1px solid rgb(145,145,145)");
    pContainer->initControls();
    pContainer->setUserAccount(m_pOrderMgr);
    return pContainer;
}

QWidget*    CSDIWindow::makeDockWidget_32()
{
    CSDIOrderFrame* pContainer = new CSDIOrderFrame();
    pContainer->setStyleSheet("border:1px solid rgb(145,145,145)");
    pContainer->initControls();
    pContainer->setUserAccount(m_pOrderMgr);
    return pContainer;
}

QWidget*    CSDIWindow::makeDockWidget_33()
{
    CSDITradeFrame* pContainer = new CSDITradeFrame();
    pContainer->setStyleSheet("border:1px solid rgb(145,145,145)");
    pContainer->initControls();
    pContainer->setUserAccount(m_pOrderMgr);
    return pContainer;
}






