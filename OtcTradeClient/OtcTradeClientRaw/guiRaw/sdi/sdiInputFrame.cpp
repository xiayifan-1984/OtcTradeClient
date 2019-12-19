

#include "sdiInputFrame.h"
#include "trademodule.h"
#include "docktitlebar.h"
#include "SmartInputBox.h"
#include "SmartTwapBox.h"
#include "SmartParkBox.h"
#include <QDebug>


CSDIInputFrame::CSDIInputFrame(QWidget *parent):
    QFrame(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    _orderMrg = nullptr;
    _titlebar = nullptr;
    _tabWidget = nullptr;
    _kbInputbox = nullptr;
    _twapInputbox = nullptr;
    _parkInputbox = nullptr;
}

CSDIInputFrame::~CSDIInputFrame()
{

}

void   CSDIInputFrame::initControls()
{
    //[0]
    _tabWidget = new QTabWidget();
    _tabWidget->setTabPosition(QTabWidget::North);
    _tabWidget->setStyleSheet("border:0px solid rgb(145,145,145)");

    //[2]
    _kbInputbox = new CSmartInputBox();
    _twapInputbox = new CSmartTwapBox();
    _parkInputbox = new CSmartParkBox();

    //[3]创建TabWidget
    {
        _tabWidget->addTab(_kbInputbox, "普通下单板");
        _tabWidget->addTab(_twapInputbox, "智能Twap单");
        _tabWidget->addTab(_parkInputbox, "条件预埋单");
    }

    //[4]创建Titlebar
    {
        _titlebar = new CDockTitlebar();
        _titlebar->setTitle(tr("普通下单板"));
        _titlebar->setReady();
    }

    //[5]布局
    {
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addWidget(_titlebar);
        mainLayout->addWidget(_tabWidget);
        this->setLayout(mainLayout);
        mainLayout->setSpacing(0);
        mainLayout->setMargin(2);
    }

    QObject::connect( _tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabSelectChanged(int))  );

}

void   CSDIInputFrame::closeEvent(QCloseEvent *event)
{

    event->accept();
}

void    CSDIInputFrame::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);

}

void    CSDIInputFrame::setUserAccount(QOrderMgr* p)
{
    _orderMrg = p;

}


void    CSDIInputFrame::onTabSelectChanged(int index)
{
    qDebug() << "CSDIInputFrame::onTabSelectChanged" << index;

    if(_titlebar == nullptr)
    {
        return;
    }

    if(index == 0)
    {
        _titlebar->setTitle(tr("普通下单板"));
    }
    else if(index ==1)
    {
        _titlebar->setTitle(tr("智能Twap单"));
    }
    else if(index == 2)
    {
        _titlebar->setTitle(tr("条件预埋单"));
    }
}


