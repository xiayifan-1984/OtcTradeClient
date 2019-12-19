
#include "sdiTradeFrame.h"
#include "configmodule.h"
#include "trademodule.h"
#include "XTCodec.h"
#include "eventcentermodule.h"
#include "tradeSingleData.h"
#include "docktitlebar.h"
#include <QDebug>

CSDITradeFrame::CSDITradeFrame(QWidget *parent) :
    QFrame(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    _orderMrg = nullptr;
    _titlebar = nullptr;
    _tabWidget = nullptr;

    _tradeWidget = nullptr;
    _totalWidget = nullptr;
}

CSDITradeFrame::~CSDITradeFrame()
{

}

void    CSDITradeFrame::initControls()
{
    //[0]
    _tabWidget = new QTabWidget();
    _tabWidget->setTabPosition(QTabWidget::South);
    _tabWidget->setStyleSheet("border:0px solid rgb(145,145,145)");


    //[1]tradeview
    {
       _tradeWidget = new CTradeSingleData();
    }

    //[2]
    {
       _totalWidget = new CTradeTotalData();
    }

    //[3]创建TabWidget
    {
        _tabWidget->addTab(_tradeWidget, "成交明细 ( F3 ) ");
        _tabWidget->addTab(_totalWidget, "成交汇总 ( F5 ) ");
    }

    //[4]创建Titlebar
    {
        _titlebar = new CDockTitlebar();
        _titlebar->setTitle(tr("成交明细"));
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

    //[6]
    QObject::connect( _tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabSelectChanged(int))  );

}

void CSDITradeFrame::closeEvent(QCloseEvent *event)
{

    event->accept();
}

void    CSDITradeFrame::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);

    if (_tradeWidget)
    {
        _tradeWidget->setUserAccount(_orderMrg);
    }
    if (_totalWidget)
    {
        _totalWidget->setUserAccount(_orderMrg);
    }

}

void    CSDITradeFrame::setUserAccount(QOrderMgr* p)
{
    _orderMrg = p;

    if (_tradeWidget)
    {
        _tradeWidget->setUserAccount(_orderMrg);
    }
    if (_totalWidget)
    {
        _totalWidget->setUserAccount(_orderMrg);
    }
}

void    CSDITradeFrame::onTabSelectChanged(int index)
{
    qDebug() << "CSDITradeFrame::onTabSelectChanged" << index;

    if(_titlebar == nullptr)
    {
        return;
    }

    if(index == 0)
    {
        _titlebar->setTitle(tr("成交明细 "));
    }
    else if(index ==1)
    {
        _titlebar->setTitle(tr("成交汇总"));
    }
}

