
#include "eventcentermodule.h"
#include <QApplication>

OrderEventArgs::OrderEventArgs(int type)
    : QEvent( Type(type) )
{
    usertype =0;       //账号信息，一个完整的账号信息包括:账号类型，经纪商号码，账号
    broker =0;
    memset(user, 0, sizeof(user));

    subevent =0;
    memset(orderref, 0, sizeof(orderref));
    hastrade = false;

    memset(reason, 0, sizeof(reason));
}

TransactionEventArgs::TransactionEventArgs(int  type)
    :QEvent( Type(type) )
{
    usertype = 0;       //账号信息，一个完整的账号信息包括:账号类型，经纪商号码，账号
    broker = 0;
    memset(user, 0, sizeof(user) );

    subevent = 0;       //成交单事件
    memset(tradeid, 0, sizeof(tradeid));
    memset(orderref, 0, sizeof(orderref));
}

SysNotifyEventArgs::SysNotifyEventArgs(int type)
    :QEvent( Type(type) )
{
    mainevent =0;      //主类型
    subevent =0;       //子类型
    val =0;            //值
    memset(sval, 0, sizeof(sval));       //补充值
}

//=================================================================================================================================================================================================================================
QEventCenter*      g_eventcenter = nullptr;

QEventCenter*      GetEventCenter()
{
    if(nullptr == g_eventcenter)
    {
        g_eventcenter = new QEventCenter();
    }

    return g_eventcenter;
}

QEventCenter::QEventCenter()
{

}

QEventCenter::~QEventCenter()
{

}

int      QEventCenter::Init()
{
     QEvent::registerEventType(CET_Order);
     QEvent::registerEventType(CET_Transact);
     QEvent::registerEventType(CET_SysNotify);

     return 1;
}

void     QEventCenter::Free()
{

}

void     QEventCenter::PostOrderMessage(OrderEventArgs* pargs)
{
    qApp->postEvent(this, pargs);
}

void     QEventCenter::PostTransactMessage(TransactionEventArgs* pargs)
{
    qApp->postEvent(this, pargs);
}

void     QEventCenter::PostSysNotifyMessage(SysNotifyEventArgs* pargs)
{
    qApp->postEvent(this, pargs);
}

void     QEventCenter::customEvent(QEvent *event)
{
    if(event->type() == static_cast<QEvent::Type>(CET_Order) )
    {
        OrderEventArgs* e = static_cast <OrderEventArgs*>(event);

        emit fireOrderEvent(e);
    }
    else if(event->type() == static_cast<QEvent::Type>(CET_Transact) )
    {
        TransactionEventArgs* e = static_cast<TransactionEventArgs*>(event);

        emit fireTransactEvent(e);
    }
    else if(event->type() == static_cast<QEvent::Type>(CET_SysNotify) )
    {
        SysNotifyEventArgs* e = static_cast<SysNotifyEventArgs*>(event);

        emit fireSysNotifyEvent(e);
    }
}


