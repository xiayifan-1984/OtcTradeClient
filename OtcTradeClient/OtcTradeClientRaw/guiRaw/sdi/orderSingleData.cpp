
#include "orderSingleData.h"
#include "twaporderdata.h"

#include "eventcentermodule.h"
#include "trademodule.h"
#include "twapordermgr.h"
#include "modeldataorder.h"
#include "parkorderdata.h"
#include "parkordermgr.h"

CTwapSingleData::CTwapSingleData(QWidget *parent)
    :QWidget (parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    _twapOrderTab = nullptr;
    _orderMrg = nullptr;

    initControls();
}

void   CTwapSingleData::closeEvent(QCloseEvent *event)
{
    QObject::disconnect(GetEventCenter(), SIGNAL(fireTwapTwapOrderEvent(TwapOrderEvent*)), this, SLOT(onOrderEvent(TwapOrderEvent*)) );
    QObject::disconnect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onInsertOrderEvent(OrderEventArgs*)) );

    event->accept();
}

void    CTwapSingleData::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);
    updateTwapOrderInfo();
}

void    CTwapSingleData::setUserAccount(QOrderMgr* p)
{
    _orderMrg = p;
    updateTwapOrderInfo();
}

void CTwapSingleData::updateTwapOrderInfo()
{
    if(nullptr == _orderMrg)
    {
        return;
    }

    int ntype =0;
    int nbroker =0;
    char szuser[32] ={0};
    _orderMrg->WhoIs(ntype, nbroker, szuser);

    char strkey[255]={0};
    sprintf(strkey, "%d_%s", nbroker, szuser);
    string key(strkey);

    auto twapMgr = GetTwapOrderMgr();

    if(twapMgr)
    {
        auto user = twapMgr->findOrderMgr(key);

        _twapOrderTab->setUserMgr(user);
    }
}

void CTwapSingleData::onDoubleClickedTableView(const QModelIndex &ind)
{
    int row = ind.row();
    int col = ind.column();

    if(nullptr == _orderMrg)
    {
        return;
    }

    if(col == 10)
    {
        _twapOrderTab->showDetailOrders(_orderMrg, row, col);
    }
    else
    {
        int ntype =0;
        int nbroker =0;
        char szuser[32] ={0};
        _orderMrg->WhoIs(ntype, nbroker, szuser);

        char strkey[255]={0};
        sprintf(strkey, "%d_%s", nbroker, szuser);
        string key(strkey);

        auto twapMgr = GetTwapOrderMgr();
        if(twapMgr)
        {
            auto user = twapMgr->findOrderMgr(key);
            if(user)
            {
                _twapOrderTab->cancelTwapOrder(user, row);
            }
        }
    }
}

void  CTwapSingleData::cancelAllOrder()
{
    if(nullptr == _orderMrg)
    {
        return;
    }

    QString strMsg;
    strMsg.sprintf("是否撤销所有Twap单?");
    QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, "撤单提醒",strMsg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        int ntype =0;
        int nbroker =0;
        char szuser[32] ={0};
        _orderMrg->WhoIs(ntype, nbroker, szuser);

        char strkey[255]={0};
        sprintf(strkey, "%d_%s", nbroker, szuser);
        string key(strkey);

        auto twapMgr = GetTwapOrderMgr();
        if(twapMgr)
        {
            auto user = twapMgr->findOrderMgr(key);
            if(user)
            {
                _twapOrderTab->cancelAllTwapOrder(user);
            }
        }
    }
}

void  CTwapSingleData::cancelOneOrder()
{
    QModelIndex index = _twapOrderTab->currentIndex();
    if(!index.isValid())
    {
        return;
    }

    int row = index.row();
    //int col = index.column();

    if(nullptr == _orderMrg)
    {
        return;
    }

    int ntype =0;
    int nbroker =0;
    char szuser[32] ={0};
    _orderMrg->WhoIs(ntype, nbroker, szuser);

    char strkey[255]={0};
    sprintf(strkey, "%d_%s", nbroker, szuser);
    string key(strkey);

    auto twapMgr = GetTwapOrderMgr();
    if(twapMgr)
    {
        auto user = twapMgr->findOrderMgr(key);
        if(user)
        {
            _twapOrderTab->cancelTwapOrder(user, row);
        }
    }
}

void CTwapSingleData::onOrderEvent(TwapOrderEvent *pEvent)
{
    if(nullptr == _orderMrg)
    {
        return;
    }

    int ntype =0;
    int nbroker =0;
    char szuser[32] ={0};
    _orderMrg->WhoIs(ntype, nbroker, szuser);

    if(nbroker == pEvent->broker && strncmp(szuser, pEvent->user, strlen(szuser)) == 0)
    {
        updateTwapOrderInfo();
        return;
    }

}

void CTwapSingleData::onInsertOrderEvent(OrderEventArgs *pEvent)
{
    if(nullptr == _orderMrg)
    {
        return;
    }

    int ntype =0;
    int nbroker =0;
    char szuser[32] ={0};
    _orderMrg->WhoIs(ntype, nbroker, szuser);

    if(nbroker == pEvent->broker && strncmp(szuser, pEvent->user, strlen(szuser)) == 0)
    {
        updateTwapOrderInfo();
        return;
    }

}


void CTwapSingleData::initControls()
{
    _twapOrderTab = new TwapOrderTable();

    //布局
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(_twapOrderTab);

    //mainLayout->setSpacing(0);
    mainLayout->setMargin(0);       //把margin 置为 0

    this->setLayout(mainLayout);


    //消息
    QObject::connect(_twapOrderTab, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onDoubleClickedTableView(const QModelIndex&)));
    QObject::connect(GetEventCenter(), SIGNAL(fireTwapTwapOrderEvent(TwapOrderEvent*)), this, SLOT(onOrderEvent(TwapOrderEvent*)) );
    QObject::connect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onInsertOrderEvent(OrderEventArgs*)) );
}




//===================================================================================================================================
//===================================================================================================================================
//===================================================================================================================================
//===================================================================================================================================
COrderSingleData::COrderSingleData(QWidget* parent):
    QWidget(parent)
{
    _tblViewNormal = nullptr;
    _modelNormal = nullptr;
    _orderMrg = nullptr;
}

void   COrderSingleData::closeEvent(QCloseEvent *event)
{
    QObject::disconnect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onOrderEvent(OrderEventArgs*)) );

    if (_modelNormal)
    {
        _modelNormal->release();
        delete _modelNormal;
        _modelNormal = nullptr;
    }

    event->accept();
}

void                COrderSingleData::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);

    if(_modelNormal)
    {
        _modelNormal->setOrderMgr(_orderMrg);
    }
}

void                COrderSingleData::setUserAccount(QOrderMgr* p)
{
    _orderMrg = p;

    if(_modelNormal)
    {
        _modelNormal->setOrderMgr(_orderMrg);
    }
}

void                COrderSingleData::cancelAllOrder()
{
    if(nullptr == _orderMrg)
    {
        return;
    }

    int ncount = _orderMrg->GetAllOrder(nullptr, 0);
    if (ncount <= 0)
    {
        return;
    }

    vector<tagXTOrderField> arrOrders;

    tagXTOrderField* pArr = new tagXTOrderField[ncount];
    _orderMrg->GetAllOrder(pArr, ncount);

    for (int i = 0; i < ncount; i++)
    {
        if(pArr[i].OrderStatus == XT_OST_Accepted)
        {
            arrOrders.push_back(pArr[i]);
        }
    }

    delete[] pArr;

    int nsize = arrOrders.size();
    if(nsize <= 0)
    {
        return;
    }

    QString strMsg;
    strMsg.sprintf("是否撤销所有委托单?");
    QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, "撤单提醒",strMsg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        for(int i=0; i < nsize; i++)
        {
            tagXTOrderField* p = &arrOrders[i];

            tagXTInputOrderActionField oField;
            memset(&oField, 0, sizeof(oField));
            oField.BrokerID = p->BrokerID;
            strcpy(oField.UserID, p->UserID);
            oField.ExCode = p->ExCode;
            strcpy(oField.ActionRef, p->OrderRef);
            oField.ActionFlag = XT_AF_Delete;
            strcpy(oField.OrderRef, p->OrderRef);
            oField.FrontID = p->FrontID;
            oField.SessionID = p->SessionID;
            strcpy(oField.OrderSysID, p->OrderSysID);

            _orderMrg->CancelOrder(&oField);
        }

    }
}

void                COrderSingleData::cancelOneOrder()
{
    QModelIndex index = _tblViewNormal->currentIndex();
    on_tableview_doubleClicked(index);
}

void                COrderSingleData::onOrderEvent(OrderEventArgs* e)
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

void                COrderSingleData::on_tableview_doubleClicked(const QModelIndex& index)
{
    if(!index.isValid())
    {
        return;
    }

    tagXTOrderField* p = nullptr;
    p = _modelNormal->getDataByRow(index);
   
    if(p)
    {
        //如果状态为XT_OST_Accepted
        if(p->OrderStatus == XT_OST_Accepted)
        {
            if(_orderMrg)
            {
                //弹出对话框确认
                QString strMsg;
                strMsg.sprintf("是否对委托单<font color='red'>[%s]</font>进行撤单?", p->OrderRef);
                QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, "撤单提醒",strMsg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                if(rb == QMessageBox::Yes)
                {
                    tagXTInputOrderActionField oField;
                    memset(&oField, 0, sizeof(oField));
                    oField.BrokerID = p->BrokerID;
                    strcpy(oField.UserID, p->UserID);
                    oField.ExCode = p->ExCode;
                    strcpy(oField.ActionRef, p->OrderRef);
                    oField.ActionFlag = XT_AF_Delete;
                    strcpy(oField.OrderRef, p->OrderRef);
                    oField.FrontID = p->FrontID;
                    oField.SessionID = p->SessionID;
                    strcpy(oField.OrderSysID, p->OrderSysID);

                    _orderMrg->CancelOrder(&oField);
                }
            }

        }
    }
}


void                COrderSingleData::initControls(int mode /* = 0 */)
{
    _tblViewNormal = new QTableView();
    _modelNormal = new ModelSingleOrder();

    if (mode == 1) //仅挂单
    {
        _modelNormal->setOrderToShow(ModelSingleOrder::ACCEPTED_ORDER);
    }
    
    _tblViewNormal->setModel(_modelNormal);

    _tblViewNormal->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
    _tblViewNormal->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    _tblViewNormal->verticalHeader()->hide();  //隐藏标号
    _tblViewNormal->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

     //[2]布局
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(_tblViewNormal);
    mainLayout->setMargin(0);       //把margin 置为 0
    this->setLayout(mainLayout);

    //[3]消息
    QObject::connect(_tblViewNormal, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_tableview_doubleClicked(const QModelIndex&)) );
    QObject::connect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onOrderEvent(OrderEventArgs*)) );
}






//===================================================================================================================================
//===================================================================================================================================
//===================================================================================================================================
//===================================================================================================================================

CParkSingleData::CParkSingleData(QWidget *parent)
    :QWidget (parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    _parkOrderTab = nullptr;
    _orderMrg = nullptr;

    initControls();
}

void   CParkSingleData::closeEvent(QCloseEvent *event)
{
    QObject::disconnect(GetEventCenter(), SIGNAL(fireParkOrderEvent(ParkOrderEvent*)), this, SLOT(onOrderEvent(ParkOrderEvent*)) );

    event->accept();
}

void    CParkSingleData::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);
    updateParkOrderInfo();
}

void    CParkSingleData::setUserAccount(QOrderMgr* p)
{
    _orderMrg = p;
    updateParkOrderInfo();
}

void    CParkSingleData::updateParkOrderInfo()
{
    if(nullptr == _orderMrg)
    {
        return;
    }

    int ntype =0;
    int nbroker =0;
    char szuser[32] ={0};
    _orderMrg->WhoIs(ntype, nbroker, szuser);

    char strkey[255]={0};
    sprintf(strkey, "%d_%s", nbroker, szuser);
    string key(strkey);

    auto parkMgr = GetParkedOrderMgr();

    if(parkMgr)
    {
        auto user = parkMgr->findMgrByUser(key);

        _parkOrderTab->setUserMgr(user);
    }
}

void CParkSingleData::onDoubleClickedTableView(const QModelIndex &ind)
{
    int row = ind.row();
    int col = ind.column();

    if(nullptr == _orderMrg)
    {
        return;
    }

    {
        int ntype =0;
        int nbroker =0;
        char szuser[32] ={0};
        _orderMrg->WhoIs(ntype, nbroker, szuser);

        char strkey[255]={0};
        sprintf(strkey, "%d_%s", nbroker, szuser);
        string key(strkey);

        auto parkMgr = GetParkedOrderMgr();
        if(parkMgr)
        {
            auto user = parkMgr->findMgrByUser(key);
            if(user)
            {
                _parkOrderTab->cancelParkOrder(user, row);
            }
        }
    }
}

void  CParkSingleData::cancelAllOrder()
{
    if(nullptr == _orderMrg)
    {
        return;
    }

    QString strMsg;
    strMsg.sprintf("是否撤销所有Park单?");
    QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, "撤单提醒",strMsg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb == QMessageBox::Yes)
    {
        int ntype =0;
        int nbroker =0;
        char szuser[32] ={0};
        _orderMrg->WhoIs(ntype, nbroker, szuser);

        char strkey[255]={0};
        sprintf(strkey, "%d_%s", nbroker, szuser);
        string key(strkey);

        auto parkMgr = GetParkedOrderMgr();
        if(parkMgr)
        {
            auto user = parkMgr->findMgrByUser(key);
            if(user)
            {
                _parkOrderTab->cancelAllParkOrder(user);
            }
        }
    }
}

void  CParkSingleData::cancelOneOrder()
{
    QModelIndex index = _parkOrderTab->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    int row = index.row();
    //int col = index.column();

    if(nullptr == _orderMrg)
    {
        return;
    }

    int ntype =0;
    int nbroker =0;
    char szuser[32] ={0};
    _orderMrg->WhoIs(ntype, nbroker, szuser);

    char strkey[255]={0};
    sprintf(strkey, "%d_%s", nbroker, szuser);
    string key(strkey);

    auto parkMgr = GetParkedOrderMgr();
    if(parkMgr)
    {
        auto user = parkMgr->findMgrByUser(key);
        if(user)
        {
            _parkOrderTab->cancelParkOrder(user, row);
        }
    }
}

void   CParkSingleData::manualSendOneOrder()
{
    QModelIndex index = _parkOrderTab->currentIndex();
    if(!index.isValid())
    {
        return;
    }
    int row = index.row();
    //int col = index.column();

    if(nullptr == _orderMrg)
    {
        return;
    }

    int ntype =0;
    int nbroker =0;
    char szuser[32] ={0};
    _orderMrg->WhoIs(ntype, nbroker, szuser);

    char strkey[255]={0};
    sprintf(strkey, "%d_%s", nbroker, szuser);
    string key(strkey);

    auto parkMgr = GetParkedOrderMgr();
    if(parkMgr)
    {
        auto user = parkMgr->findMgrByUser(key);
        if(user)
        {
            _parkOrderTab->manualSendParkOrder(user, row);
        }
    }
}

void CParkSingleData::onOrderEvent(ParkOrderEvent *pEvent)
{
    if(nullptr == _orderMrg)
    {
        return;
    }

    int ntype =0;
    int nbroker =0;
    char szuser[32] ={0};
    _orderMrg->WhoIs(ntype, nbroker, szuser);

    if(nbroker == pEvent->broker && strncmp(szuser, pEvent->user, strlen(szuser)) == 0)
    {
        updateParkOrderInfo();
        return;
    }

}


void CParkSingleData::initControls()
{
    _parkOrderTab = new ParkOrderTable();

    //布局
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(_parkOrderTab);

    //mainLayout->setSpacing(0);
    mainLayout->setMargin(0);       //把margin 置为 0

    this->setLayout(mainLayout);


    //消息
    QObject::connect(_parkOrderTab, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onDoubleClickedTableView(const QModelIndex&)));
    QObject::connect(GetEventCenter(), SIGNAL(fireParkOrderEvent(ParkOrderEvent*)), this, SLOT(onOrderEvent(ParkOrderEvent*)) );

}

