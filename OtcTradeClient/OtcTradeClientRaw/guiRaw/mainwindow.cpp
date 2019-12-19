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
#include "sdiwindow.h"

#include "XTCodec.h"
#include "trademodule.h"
#include "configmodule.h"

#include "debugquote.h"
#include "selectcommodity.h"
#include "selectmaincontract.h"

#include "datanametable.h"
#include "datastaticdata.h"
#include "dataorder.h"
#include "mditradedata.h"
#include "mdipositiondata.h"

#include "parkorderdata.h"
#include "twaporderdata.h"

#include "otcRiskAvoid.h"
#include "otcInquiryBox.h"
#include "otcriskcalc.h"
#include "kbinputbox.h"
#include "SmartParkBox.h"
#include "twapinputbox.h"


CMainWindow::CMainWindow()
    : mdiArea(new QMdiArea)
{
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);


    createActions();
    createStatusbar();

    setWindowTitle(tr("OtcTradeClient(MDI)"));
    setUnifiedTitleAndToolBarOnMac(true);

    setAttribute(Qt::WA_DeleteOnClose);

}

void CMainWindow::closeEvent(QCloseEvent *event)
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
    }
}

void CMainWindow::about()
{
   QMessageBox::about(this, tr("About OtcTradeClient"),
            tr("The <b>OtcTradeClient</b> demonstrates a otc system, and how to trade and analysis future and option using Qt. "
             ));
}

void CMainWindow::createActions()
{
    QMenu *sysMenu = menuBar()->addMenu(tr("&System"));
    {
        QAction* mdiContainer = sysMenu->addAction(tr("MDI Frame"), this , &CMainWindow::ShowMDIFrame );
        mdiContainer->setStatusTip(tr("显示一个MDI框架"));
        mdiContainer->setIcon(QIcon(":/image/400.bmp"));

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

        QAction* new1= sysMenu->addAction(tr("Configuration"), this, &CMainWindow::ShowConfiguration);
        new1->setStatusTip(tr("显示系统设置"));
        new1->setIcon(QIcon(":/image/001.bmp"));
        sysMenu->addSeparator();

        QAction* new2= sysMenu->addAction(tr("Close"), this, &CMainWindow::ShowExit );
        new2->setIcon(QIcon(":/image/exit.bmp"));
        new2->setStatusTip(tr("关闭"));
    }

    menuBar()->addMenu(tr("&Analysis"));
    {
        //QMenu *analysisMenu =
        //@@@@@@@@@@@@@
    }

    QMenu *tradeMenu = menuBar()->addMenu(tr("View"));
    {
        QAction* new1 = tradeMenu->addAction(tr("OtcRiskAvoid"), this, &CMainWindow::showOtcRiskAvoid);
        new1->setStatusTip(tr("显示OTC避险画面"));

        QAction* new3 = tradeMenu->addAction(tr("OtcInquiry"), this, &CMainWindow::showOtcInquiry);
        new3->setStatusTip(tr("显示OTC持仓"));

        QAction* new4 = tradeMenu->addAction(tr("OtcRiskCalculation"), this, &CMainWindow::showOtcRiskCalculation);
        new4->setStatusTip(tr("显示OTC计算器"));

        QAction* new2 = tradeMenu->addAction(tr("KBInputBox"), this, &CMainWindow::showKBInputBox);
        new2->setStatusTip(tr("显示下单板"));

        QAction* new5 = tradeMenu->addAction(tr("ParkOrderInputBox"), this, &CMainWindow::showParkOrderInputBox);
        new5->setStatusTip(tr("显示条件单下单板"));

        QAction* new6 = tradeMenu->addAction(tr("TwapInputBox"), this, &CMainWindow::showTwapInputBox);
        new6->setStatusTip(tr("显示Twap下单板"));

    }

    QMenu *dataMenu = menuBar()->addMenu(tr("&Data"));
    {
        QAction* newmdi = dataMenu->addAction(tr("NameTable"), this, &CMainWindow::showDataNameTable);
        newmdi->setStatusTip(tr("显示名称代码表"));

        QAction* newmdi2 = dataMenu->addAction(tr("StaticData"), this, &CMainWindow::showDataStaticData);
        newmdi2->setStatusTip(tr("显示静态数据"));

        QAction* newmdi3 = dataMenu->addAction(tr("OrderRecord"), this, &CMainWindow::showDataOrderRecord);
        newmdi3->setStatusTip(tr("显示委托报表"));

        QAction* newmdi4 = dataMenu->addAction(tr("TradeRecord"), this, &CMainWindow::showDataTradeRecord);
        newmdi4->setStatusTip(tr("显示成交报表"));

        QAction* newmdi5 = dataMenu->addAction(tr("PositionRecord"), this, &CMainWindow::showDataPositionRecord);
        newmdi5->setStatusTip(tr("显示持仓报表"));

        QAction* newmdi6 = dataMenu->addAction(tr("ParkOrderTab"), this, &CMainWindow::showParkOrderData);
        newmdi6->setStatusTip(tr("显示条件/预埋报表"));

        QAction* newmdi7 = dataMenu->addAction(tr("TwapOrderTab"), this, &CMainWindow::showTwapOrderData);
        newmdi7->setStatusTip(tr("显示Twap报表"));
    }

    QMenu* debugMenu = menuBar()->addMenu(tr("Debug"));
    {
        QAction* newmdi3 = debugMenu->addAction(tr("Quote"), this, &CMainWindow::showDebugQuote);
        newmdi3->setStatusTip(tr("显示行情报表"));

        QAction* newmdi7 = debugMenu->addAction(tr("Select MainContract"), this, &CMainWindow::showSelectMainContract);
        newmdi7->setStatusTip(tr("显示主力合约对话框"));

        QAction* newmdi8 = debugMenu->addAction(tr("Select Commodity"), this, &CMainWindow::showSelectCommodity);
        newmdi8->setStatusTip(tr("显示选择合约对话框"));

    }

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    {
        QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &CMainWindow::about);
        aboutAct->setStatusTip(tr("Show the application's About box"));

        QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
        aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    }

}

void CMainWindow::createStatusbar()
{
    statusBar()->showMessage(tr("Ready"));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//[Data]菜单

//显示 名称代码表
void    CMainWindow::showDataNameTable()
{
    DataNameTable* child = new DataNameTable();
    mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Name Table"));
    child->show();
    mdiArea->activeSubWindow()->resize(600,500);
}

//显示 静态数据
void    CMainWindow::showDataStaticData()
{
    DataStaticData* child = new DataStaticData();
    mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Static Data"));
    child->show();
}

//显示 委托单记录
void    CMainWindow::showDataOrderRecord()
{
    MultiOrderWidget* child = new MultiOrderWidget();
    mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Order Record"));
    child->show();
    mdiArea->activeSubWindow()->resize(800,400);
}

//显示 成交记录
void     CMainWindow::showDataTradeRecord()
{
    CMDITradeData* child = new CMDITradeData();
    mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Trade Record"));
    child->show();
    mdiArea->activeSubWindow()->resize(800,400);
}

//显示 交易持仓
void     CMainWindow::showDataPositionRecord()
{
    CMDIPositionData* child = new CMDIPositionData();
    mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Position Record"));
    child->show();
    mdiArea->activeSubWindow()->resize(800,400);
}

void CMainWindow::showParkOrderData()
{
    auto child = new CParkOrderData();
    mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("park and condition"));
    child->show();
    mdiArea->activeSubWindow()->resize(1200,200);
}

void CMainWindow::showTwapOrderData()
{
    auto child = new CTwapOrderData();
    mdiArea->addSubWindow(child);
    child->setWindowTitle(tr("TwapOrderData"));
    child->show();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//[view]菜单

void     CMainWindow::showOtcRiskAvoid()
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

void    CMainWindow::showOtcInquiry()
{
    //询价与成交
    ViewInquiryBox* child = new ViewInquiryBox();
    mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Inquiry Box"));
    child->show();
    mdiArea->activeSubWindow()->resize(1600,200);
}

void    CMainWindow::showKBInputBox()
{
    KBInputBox* child = new KBInputBox();
    mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("KBInputbox"));
    child->show();
}

void CMainWindow::showParkOrderInputBox()
{
    CSmartParkBox* child = new CSmartParkBox();
    mdiArea->addSubWindow(child);
    child->setWindowTitle(tr("ParkAndConditionBox"));
    child->show();
    mdiArea->activeSubWindow()->resize(480,320);
}

void CMainWindow::showTwapInputBox()
{
    auto child = new CTwapInputBox();
    mdiArea->addSubWindow(child);
    child->setWindowTitle(tr("TwapInput"));
    child->show();
}

void CMainWindow::showOtcRiskCalculation()
{
    OtcRiskCalc* child = new OtcRiskCalc();
    mdiArea->addSubWindow(child);
    child->setWindowTitle(tr("OtcRiskCaculationTab"));
    child->show();
    mdiArea->activeSubWindow()->resize(800,200);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//[Debug]菜单
void    CMainWindow::showDebugQuote()
{
    DebugQuote* child = new DebugQuote();
    mdiArea->addSubWindow(child);

    child->setWindowTitle(tr("Show Quote"));
    child->show();
}

void      CMainWindow::showSelectCommodity()
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

void    CMainWindow::showSelectMainContract()
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//[System] 菜单
void      CMainWindow::ShowMDIFrame()
{
    CMainWindow* pnew = new CMainWindow();
    pnew->show();

}

void CMainWindow::ShowSDIUser(QAction* pAction)
{
    CSDIWindow* pnew = new CSDIWindow();
    pnew->setSDIUser( pAction->data(), pAction->text() );
    pnew->initControls();
    pnew->showMaximized();
}

void      CMainWindow::ShowConfiguration()
{
    //@@@@@@@@
}

void      CMainWindow::ShowExit()
{
    close();
}


