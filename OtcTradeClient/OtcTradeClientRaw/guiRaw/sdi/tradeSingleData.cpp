
#include "tradeSingleData.h"
#include "eventcentermodule.h"
#include "trademodule.h"
#include "modeldatatrade.h"

CTradeSingleData::CTradeSingleData(QWidget* parent ):
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    _tblViewNormal = nullptr;
    _modelNormal = nullptr;
    _orderMrg = nullptr;

    initControls();
}

void     CTradeSingleData::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);
    if (_modelNormal)
    {
        _modelNormal->setOrderMgr(_orderMrg);
    }
}

void     CTradeSingleData::setUserAccount(QOrderMgr* p)
{
    _orderMrg = p;

    if (_modelNormal)
    {
        _modelNormal->setOrderMgr(_orderMrg);
    }
}

void     CTradeSingleData::closeEvent(QCloseEvent *event)
{
    QObject::disconnect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );

    if (_modelNormal)
    {
        _modelNormal->release();
        delete _modelNormal;
        _modelNormal = nullptr;
    }
    
    event->accept();
}


void     CTradeSingleData::onTransactEvent(TransactionEventArgs* e)
{
    if(_orderMrg)
    {
        int usertype =0;
        int broker=0;
        char szuser[32]={0};
        _orderMrg->WhoIs(usertype, broker, szuser);

        if(e->broker == broker &&
                e->usertype == usertype &&
                stricmp(e->user, szuser) == 0
          )
        {
            _modelNormal->setOrderMgr(_orderMrg);
        }

    }
}


void     CTradeSingleData::initControls()
{
    _tblViewNormal = new QTableView();

    _modelNormal = new ModelSingleTrade();
        _tblViewNormal->setModel(_modelNormal);

        _tblViewNormal->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tblViewNormal->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tblViewNormal->verticalHeader()->hide();  //隐藏标号
        _tblViewNormal->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        //设置列宽
        _tblViewNormal->setColumnWidth(0, 80);
        _tblViewNormal->setColumnWidth(1, 80);
        _tblViewNormal->setColumnWidth(2, 80);
        _tblViewNormal->setColumnWidth(3, 80);

        _tblViewNormal->setColumnWidth(5, 80);
        _tblViewNormal->setColumnWidth(6, 80);
        _tblViewNormal->setColumnWidth(9, 80);

    //布局
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(_tblViewNormal);
    mainLayout->setMargin(0);       //把margin 置为 0
    this->setLayout(mainLayout);  

    //消息
    QObject::connect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );  
}



//=====================================================================================================================================================================
//=====================================================================================================================================================================
//=====================================================================================================================================================================
//=====================================================================================================================================================================

CTradeTotalData::CTradeTotalData(QWidget* parent ):
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    _tblViewNormal = nullptr;
    _modelNormal = nullptr;
    _orderMrg = nullptr;

    initControls();
}

void     CTradeTotalData::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);
    if (_modelNormal)
    {
        _modelNormal->setOrderMgr(_orderMrg);
    }
}

void     CTradeTotalData::setUserAccount(QOrderMgr* p)
{
    _orderMrg = p;

    if (_modelNormal)
    {
        _modelNormal->setOrderMgr(_orderMrg);
    }
}

void     CTradeTotalData::closeEvent(QCloseEvent *event)
{
    QObject::disconnect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );

    if (_modelNormal)
    {
        _modelNormal->release();
        delete _modelNormal;
        _modelNormal = nullptr;
    }

    event->accept();
}


void     CTradeTotalData::onTransactEvent(TransactionEventArgs* e)
{
    if(_orderMrg)
    {
        int usertype =0;
        int broker=0;
        char szuser[32]={0};
        _orderMrg->WhoIs(usertype, broker, szuser);

        if(e->broker == broker &&
                e->usertype == usertype &&
                stricmp(e->user, szuser) == 0
          )
        {
            _modelNormal->setOrderMgr(_orderMrg);
        }

    }
}


void     CTradeTotalData::initControls()
{
    _tblViewNormal = new QTableView();

    _modelNormal = new ModelTotalTrade();
    _tblViewNormal->setModel(_modelNormal);
    _tblViewNormal->setSortingEnabled(true);        //可以排序

    _tblViewNormal->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
    _tblViewNormal->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    _tblViewNormal->verticalHeader()->hide();  //隐藏标号
    _tblViewNormal->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

    //设置列宽
    _tblViewNormal->setColumnWidth(0, 80);
    _tblViewNormal->setColumnWidth(1, 80);
    _tblViewNormal->setColumnWidth(2, 80);
    _tblViewNormal->setColumnWidth(3, 80);

    _tblViewNormal->setColumnWidth(5, 80);
    _tblViewNormal->setColumnWidth(6, 80);
    _tblViewNormal->setColumnWidth(9, 80);

    //布局
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(_tblViewNormal);
    mainLayout->setMargin(0);       //把margin 置为 0
    this->setLayout(mainLayout);

    //消息
    QObject::connect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );
}



























