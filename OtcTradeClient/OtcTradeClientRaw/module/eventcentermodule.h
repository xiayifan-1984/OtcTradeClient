#ifndef NOTIFYMODULE_DEFINE_H
#define NOTIFYMODULE_DEFINE_H

#include <QEvent>
#include <QObject>

//=========================================================================================================================================================================================================
//=========================================================================================================================================================================================================
//=========================================================================================================================================================================================================
enum CustomEventType
{
    CET_Order = 1001,
    CET_Transact,
    CET_ParkOrder,
    CET_TwapTwap,
    CET_SysNotify,
};


//委托单事件
#define		ORDER_EVENT_NEW				0				//新单
#define		ORDER_EVENT_UPDATE			1				//委托状态变化(可能有成交的变化)
#define		ORDER_EVENT_FAIL			2				//委托单失败

class OrderEventArgs: public QEvent
{
public:
    explicit OrderEventArgs(int type);

public:
    int             usertype;       //账号信息，一个完整的账号信息包括:账号类型，经纪商号码，账号
    int             broker;
    char            user[32];

    int             subevent;     //委托单事件
    char            orderref[16];   //OrderRef
    bool            hastrade;       //如果是Update, 是否有成交
    char            reason[32];     //如果失败，失败的原因
};

//成交单事件
#define		TRANSACT_EVENT_NEW			0				//新成交

class TransactionEventArgs: public QEvent
{
public:
    explicit  TransactionEventArgs(int type);

public:
    int             usertype;       //账号信息，一个完整的账号信息包括:账号类型，经纪商号码，账号
    int             broker;
    char            user[32];

    int             subevent;       //成交单事件
    char            tradeid[24];
    char            orderref[16];
};

#define PARK_ORDER_NEW      0
#define PARK_ORDER_ACCEPT   1
#define PARK_ORDER_INSERT   2
#define PARK_ORDER_CANCEL   3
#define PARK_ORDER_FAIL     4

class ParkOrderEvent: public QEvent
{
public:
    explicit ParkOrderEvent(int type);
public:
    int             usertype;       //账号信息，一个完整的账号信息包括:账号类型，经纪商号码，账号
    int             broker;
    char            user[32];

    int             subevent;     //委托单事件
    int             indexId;      //indicate id of park order
    char            reason[32];     //如果失败，失败的原因
};


class TwapOrderEvent: public QEvent
{
public:
    explicit TwapOrderEvent(int type);
public:
    int             usertype;       //账号信息，一个完整的账号信息包括:账号类型，经纪商号码，账号
    int             broker;
    char            user[32];

    int             subevent;     //委托单事件
    int             indexId;      //indicate id of park order
    char            reason[32];     //如果失败，失败的原因
};

//系统通知
class SysNotifyEventArgs: public QEvent
{
public:
    explicit  SysNotifyEventArgs(int type);

public:
    int             mainevent;      //主类型
    int             subevent;       //子类型
    int             val;            //值
    char            sval[32];       //补充值
};

//=========================================================================================================================================================================================================
//=========================================================================================================================================================================================================
//=========================================================================================================================================================================================================
class QEventCenter: public QObject
{
    Q_OBJECT

public:
    QEventCenter();
    virtual ~QEventCenter();

public:
    int                         Init();
    void                        Free();

signals:
    void                        fireOrderEvent(OrderEventArgs* parg);
    void                        fireTransactEvent(TransactionEventArgs* parg);
    void                        fireParkOrderEvent(ParkOrderEvent* event);
    void                        fireTwapTwapOrderEvent(TwapOrderEvent* event);
    void                        fireSysNotifyEvent(SysNotifyEventArgs* parg);

public:
    void                        PostOrderMessage(OrderEventArgs* pargs);
    void                        PostTransactMessage(TransactionEventArgs* pargs);
    void                        PostParkOrderMessage(ParkOrderEvent* event);
    void                        PostTwapOrderMessage(TwapOrderEvent* event);
    void                        PostSysNotifyMessage(SysNotifyEventArgs* pargs);

protected:
    void                        customEvent(QEvent *event);

};

extern QEventCenter*      GetEventCenter();






#endif
