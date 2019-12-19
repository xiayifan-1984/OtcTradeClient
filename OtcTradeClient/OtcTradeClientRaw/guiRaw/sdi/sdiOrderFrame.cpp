
#include "sdiOrderFrame.h"
#include "configmodule.h"
#include "eventcentermodule.h"
#include "trademodule.h"
#include "XTCodec.h"
#include "docktitlebar.h"
#include "orderSingleData.h"
#include <QDebug>

#define     TITLEBUTTON_ALLCANCEL       555
#define     TITLEBUTTON_ONECANCEL       556
#define     TITLEBUTTON_SEND            557

CSDIOrderFrame::CSDIOrderFrame(QWidget *parent):
    QFrame(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    _orderMrg = nullptr;
    _titlebar = nullptr;

    _tabWidget = nullptr;

    _acceptWidget = nullptr;
    _allorderWidget = nullptr;

    _twapWidget = nullptr;
    _parkWidget = nullptr;
}

CSDIOrderFrame::~CSDIOrderFrame()
{

}

void   CSDIOrderFrame::initControls()
{
    //[0]
    _tabWidget = new QTabWidget();
    _tabWidget->setTabPosition(QTabWidget::South);
    _tabWidget->setStyleSheet("border:0px solid rgb(145,145,145)");


    //[1]accept
    {
        _acceptWidget = new COrderSingleData();
        _acceptWidget->initControls(1);
    }

    //[2]all order
    {
       _allorderWidget = new COrderSingleData();
       _allorderWidget->initControls(0);
    }

    //[3]
    {
        _twapWidget = new CTwapSingleData();
        _parkWidget = new CParkSingleData();
    }

    //[4]创建TabWidget
    {
        _tabWidget->addTab(_acceptWidget, " 挂 单  ( F1 ) ");
        _tabWidget->addTab(_allorderWidget, "委托信息 ( F2 ) ");
        _tabWidget->addTab(_twapWidget, "T W A P 单");
        _tabWidget->addTab(_parkWidget, "条件预埋单");
    }

    //[5]创建Titlebar
    {
        _titlebar = new CDockTitlebar();
        _titlebar->setTitle(tr(" 挂 单 "));
        _titlebar->addText(TITLEBUTTON_ONECANCEL, tr("删除"));
        _titlebar->addText(TITLEBUTTON_ALLCANCEL, tr("全删"));
        _titlebar->addText(TITLEBUTTON_SEND, tr("发送"));
        _titlebar->setReady();

        _titlebar->ShowHideButton(TITLEBUTTON_SEND, false);   // 发送 按钮 不显示

        QObject::connect(_titlebar, SIGNAL(titleButtonClick(int)), this, SLOT(onTitleButtonClick(int)) );
    }

    //[6]布局
    {
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addWidget(_titlebar);
        mainLayout->addWidget(_tabWidget);
        this->setLayout(mainLayout);
        mainLayout->setSpacing(0);
        mainLayout->setMargin(2);
    }
   
    //[7]
    QObject::connect( _tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabSelectChanged(int))  );
    
}

void   CSDIOrderFrame::closeEvent(QCloseEvent *event)
{
    QObject::disconnect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onOrderEvent(OrderEventArgs*)) );

    event->accept();
}



void    CSDIOrderFrame::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);

    _acceptWidget->setUserAccount(_orderMrg);
    _allorderWidget->setUserAccount(_orderMrg);
    _twapWidget->setUserAccount(_orderMrg);
    _parkWidget->setUserAccount(_orderMrg);

}

void    CSDIOrderFrame::setUserAccount(QOrderMgr* p)
{
    _orderMrg = p;

    _acceptWidget->setUserAccount(_orderMrg);
    _allorderWidget->setUserAccount(_orderMrg);
    _twapWidget->setUserAccount(_orderMrg);
    _parkWidget->setUserAccount(_orderMrg);
}


void    CSDIOrderFrame::onTabSelectChanged(int index)
{
    qDebug() << "CSDIOrderFrame::onTabSelectChanged" << index;

    if(_titlebar == nullptr)
    {
        return;
    }

    if(index == 0)
    {
        _titlebar->setTitle(tr(" 挂 单 "));
    }
    else if(index ==1)
    {
        _titlebar->setTitle(tr("委托信息"));
    }
    else if(index == 2)
    {
        _titlebar->setTitle(tr("T W A P 单"));
    }
    else if(index == 3)
    {
        _titlebar->setTitle(tr("条件预埋单"));
    }

    //
    if(index ==3)
    {
        _titlebar->ShowHideButton(TITLEBUTTON_SEND, true);
    }
    else
    {
        _titlebar->ShowHideButton(TITLEBUTTON_SEND, false);
    }
}


void     CSDIOrderFrame::onTitleButtonClick(int buttonID)
{
    if(nullptr == _orderMrg)
    {
        return;
    }

    if(buttonID == TITLEBUTTON_ALLCANCEL)  //全撤
    {
        if(_tabWidget->currentIndex() == 0 )
        {
            _acceptWidget->cancelAllOrder();
        }
        else if(_tabWidget->currentIndex() == 1)
        {
            _allorderWidget->cancelAllOrder();
        }
        else if(_tabWidget->currentIndex() == 2)
        {
            _twapWidget->cancelAllOrder();
        }
        else if(_tabWidget->currentIndex() == 3)
        {
            _parkWidget->cancelAllOrder();
        }
    }
    else if(buttonID == TITLEBUTTON_ONECANCEL)  //撤单（一个）
    {
        if(_tabWidget->currentIndex() == 0)
        {
            _acceptWidget->cancelOneOrder();
        }
        else if(_tabWidget->currentIndex() == 1)
        {
            _allorderWidget->cancelOneOrder();
        }
        else if(_tabWidget->currentIndex() == 2)
        {
            _twapWidget->cancelOneOrder();
        }
        else if(_tabWidget->currentIndex() == 3)
        {
            _parkWidget->cancelOneOrder();
        }
    }
    else if(buttonID == TITLEBUTTON_SEND) //发送
    {
        if(_tabWidget->currentIndex() == 3)
        {
            _parkWidget->manualSendOneOrder();
        }
    }

}


