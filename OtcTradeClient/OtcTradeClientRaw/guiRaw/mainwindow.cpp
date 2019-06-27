/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "mainwindow.h"
#include "debugquote.h"

#include "selectcommodity.h"
#include "selectmaincontract.h"
#include "kbinputbox.h"
#include "datanametable.h"
#include "datastaticdata.h"
#include "dataorder.h"
#include "datatrade.h"
#include "dataposition.h"
#include "viewRiskAvoid.h"
#include "viewInquiryBox.h"
#include "internalmessaging.h"
#include <utility>
#include <thread>

extern shared_ptr<KBInputBox> tradeBox;
extern InternalSenderReceiver* GetInternalMsgSenderReceiver();

QMdiArea * G_midArea = nullptr;

MainWindow::MainWindow()
    : mdiArea(new QMdiArea)
{
    G_midArea = mdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);


    createActions();
    createStatusbar();

    setWindowTitle(tr("OtcTradeClient(MDI)"));
    setUnifiedTitleAndToolBarOnMac(true);

    setAttribute(Qt::WA_DeleteOnClose);

    tradeBox = make_shared<KBInputBox>();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow())
    {
        event->ignore();
    }
    else
    {
        //writeSettings();
        event->accept();
        if(tradeBox)
            tradeBox->close();
    }
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About OtcTradeClient"),
            tr("The <b>OtcTradeClient</b> demonstrates a otc system, and how to trade and analysis future and option using Qt. "
             ));
}

void MainWindow::createActions()
{
    QMenu *sysMenu = menuBar()->addMenu(tr("&System"));
    {
        QAction* mdiContainer = sysMenu->addAction(tr("MDIContainer"), this , &MainWindow::ShowMDIContainer );
        mdiContainer->setStatusTip(tr("Show a MDI Container"));
        mdiContainer->setIcon(QIcon(":/image/400.bmp"));

        QAction* dockContainer = sysMenu->addAction(tr("DockContainer"), this , &MainWindow::ShowDockContainer );
        dockContainer->setStatusTip(tr("Show a Split Container"));
        dockContainer->setIcon(QIcon(":/image/406.bmp"));

        QAction* new1= sysMenu->addAction(tr("Configuration"), this, &MainWindow::ShowConfiguration);
        new1->setStatusTip(tr("Show a new Configuration"));
        new1->setIcon(QIcon(":/image/001.bmp"));
        sysMenu->addSeparator();

        QAction* new2= sysMenu->addAction(tr("Close"), this, &MainWindow::ShowExit );
        new2->setIcon(QIcon(":/image/exit.bmp"));
        new2->setStatusTip(tr("Close current Window"));
    }

    QMenu *analysisMenu = menuBar()->addMenu(tr("&Analysis"));
    {
        //@@@@@@@@@@@@@
    }

    QMenu *tradeMenu = menuBar()->addMenu(tr("View"));
    {
        QAction* new1 = tradeMenu->addAction(tr("RiskAvoid"), this, &MainWindow::showRiskAvoid);
        new1->setStatusTip(tr("Show a new RiskAvoid window"));

        QAction* new2 = tradeMenu->addAction(tr("SimpleOrderBox"), this, &MainWindow::showSimpleOrderBox);
        new2->setStatusTip(tr("Show a new OrderBox window"));

        QAction* new3 = tradeMenu->addAction(tr("Inquiry"), this, &MainWindow::showInquiry);
        new3->setStatusTip(tr("Show a new Inquiry window"));

      //  QAction* new4 = tradeMenu->addAction(tr("ParkOrderBox"), this, &MainWindow::showParkOrderBox);
      //  new4->setStatusTip(tr("Show a new ParkOrderBox window"));
        //@@@@@@@@@@@@@
    }

    QMenu *dataMenu = menuBar()->addMenu(tr("&Data"));
    {
        QAction* newmdi = dataMenu->addAction(tr("NameTable"), this, &MainWindow::showDataNameTable);
        newmdi->setStatusTip(tr("Show a new NameTable window"));

        QAction* newmdi2 = dataMenu->addAction(tr("StaticData"), this, &MainWindow::showDataStaticData);
        newmdi2->setStatusTip(tr("Show a new StaticData window"));

        QAction* newmdi3 = dataMenu->addAction(tr("OrderRecord"), this, &MainWindow::showDataOrderRecord);
        newmdi3->setStatusTip(tr("Show a new Order window"));

        QAction* newmdi4 = dataMenu->addAction(tr("TradeRecord"), this, &MainWindow::showDataTradeRecord);
        newmdi4->setStatusTip(tr("Show a new Trade window"));

        QAction* newmdi5 = dataMenu->addAction(tr("PositionRecord"), this, &MainWindow::showDataPositionRecord);
        newmdi5->setStatusTip(tr("Show a new Position window"));
    }

    QMenu* debugMenu = menuBar()->addMenu(tr("Debug"));
    {
        QAction* newmdi3 = debugMenu->addAction(tr("Quote"), this, &MainWindow::showDebugQuote);
        newmdi3->setStatusTip(tr("Show a new Quote window"));

        QAction* newmdi7 = debugMenu->addAction(tr("Select MainContract"), this, &MainWindow::showSelectMainContract);
        newmdi7->setStatusTip(tr("Show a new Select MainContract window"));

        QAction* newmdi8 = debugMenu->addAction(tr("Select Commodity"), this, &MainWindow::showSelectCommodity);
        newmdi8->setStatusTip(tr("Show a new Select window"));

        QAction* newmdi9 = debugMenu->addAction(tr("KBInputBox"), this, &MainWindow::showKBInputBox);
        newmdi9->setStatusTip(tr("Show a new KBInputbox window"));
    }

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    {
        QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
        aboutAct->setStatusTip(tr("Show the application's About box"));

        QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
        aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    }

}

void MainWindow::createStatusbar()
{
    statusBar()->showMessage(tr("Ready"));
}

//显示 名称代码表
void    MainWindow::showDataNameTable()
{
    DataNameTable* child = new DataNameTable();
    QMdiSubWindow* sub = mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Name Table"));
    child->show();
    mdiArea->activeSubWindow()->resize(600,500);
}

//显示 静态数据
void    MainWindow::showDataStaticData()
{
    DataStaticData* child = new DataStaticData();
    QMdiSubWindow* sub = mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Static Data"));
    child->show();
}

//显示 委托单记录
void    MainWindow::showDataOrderRecord()
{
    MultiOrderWidget* child = new MultiOrderWidget();
    QMdiSubWindow* sub = mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Order Record"));
    child->show();
    mdiArea->activeSubWindow()->resize(800,400);
}

//显示 成交记录
void     MainWindow::showDataTradeRecord()
{
    MultiTradeWidget* child = new MultiTradeWidget();
    QMdiSubWindow* sub = mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Trade Record"));
    child->show();
    mdiArea->activeSubWindow()->resize(800,400);
}

//显示 交易持仓
void     MainWindow::showDataPositionRecord()
{
    MultiPositionWidget* child = new MultiPositionWidget();
    QMdiSubWindow* sub = mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Position Record"));
    child->show();
    mdiArea->activeSubWindow()->resize(800,400);
}

//显示 行情
void    MainWindow::showDebugQuote()
{
    DebugQuote* child = new DebugQuote();
    QMdiSubWindow* sub = mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Show Quote"));
    child->show();
}

void     MainWindow::showRiskAvoid()
{
    ViewRiskAvoid* child = new ViewRiskAvoid();
    QMdiSubWindow* sub = mdiArea->addSubWindow(child);
    auto p = child->getRecommendPostion();
    sub->move(p.first, p.second);
    qDebug()<< "Risk window x " << sub->geometry().x() << " y " << sub->geometry().y();

    child->setWindowTitle(tr("Risk Avoid"));
    child->show();
    mdiArea->activeSubWindow()->resize(800,200);
}

void     MainWindow::showSimpleOrderBox()
{
    showKBInputBox();
}

void    MainWindow::showInquiry()
{
    //询价与成交
    ViewInquiryBox* child = new ViewInquiryBox();
    QMdiSubWindow* sub = mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Inquiry Box"));
    child->show();
    mdiArea->activeSubWindow()->resize(1600,200);
}

void MainWindow::showParkOrderBox()
{
    //reserved for parked order
}

void      MainWindow::ShowMDIContainer()
{
    MainWindow* pnew = new MainWindow();
    pnew->show();

}

void      MainWindow::ShowDockContainer()
{
    //@@@@@@@@@@

}

void      MainWindow::ShowConfiguration()
{
    //@@@@@@@@
}

void      MainWindow::ShowExit()
{
    close();
}

void      MainWindow::showSelectCommodity()
{
    SelectCommodity  dlg;

    int rec = dlg.exec();
    if (rec == QDialog::Accepted)
    {
        tagXTInstrument obj;
        dlg.getSelectCommodity(&obj);
        qDebug() << "select commodity[" << obj.ExchID <<"]" << obj.Code;
    }

}

void    MainWindow::showSelectMainContract()
{
    SelectMainContract dlg;
    int rec = dlg.exec();
    if (rec == QDialog::Accepted)
    {
        tagXTInstrument obj;
        dlg.getSelectCommodity(&obj);
        qDebug() << "select commodity[" << obj.ExchID <<"]" << obj.Code;
    }
}

void    MainWindow::showKBInputBox()
{
    KBInputBox* child = new KBInputBox();
    QMdiSubWindow* sub = mdiArea->addSubWindow(child);
    int tid = GetCurrentThreadId();
    qDebug()<<"main kinput id " << tid;
    child->setWindowTitle(tr("KBInputbox"));
    child->show();
}
