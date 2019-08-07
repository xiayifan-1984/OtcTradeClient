#include "dataorder.h"
#include "configmodule.h"
#include "eventcentermodule.h"
#include "trademodule.h"
#include "XTCodec.h"
#include "modeldataorder.h"

//============================================================================================================================================================================================================================
MultiOrderWidget::MultiOrderWidget(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    initControls();

}

MultiOrderWidget::~MultiOrderWidget()
{

}

void    MultiOrderWidget::initControls()
{
    //[0]
    _label = new QLabel();
    _combox = new QComboBox();

    _tabWidget = new QTabWidget();
    _tableview = new QTableView();
    _tableViewAccepetedOrders = new QTableView();
    _tableViewParkedOrders = new QTableView();

    //[1]
    {
        _label->setText(tr("Account:"));

        _combox->setEditable(false);
        _combox->setView(new QListView());

        vector<tagOneTradeUser> arrUser;
        GetConfigModule()->GetAllTradeUser(arrUser);
        int ncount = arrUser.size();
        for (int i = 0; i < ncount; i++)
        {
            tagOneTradeUser& o = arrUser[i];

            QOrderMgr* p = GetTradeModule()->GetMgr(o.type, o.broker, o.user);

            QString strName;
            std::string strmsg = o.aliasuser;
            strName = XTCodec::AfGbk_ToQString(strmsg);

            QVariant v = QVariant::fromValue((void *)p);
            _combox->addItem(strName, v);
        }

    }

    //[2]tableview
    {
        _customModel = new ModelSingleOrder();
        _tableview->setModel(_customModel);

        _tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tableview->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tableview->verticalHeader()->hide();
        _tableview->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        _acceptedOrders = new ModelSingleOrder();
        _acceptedOrders->setOrderToShow(ORDER_TO_SHOW::ACCEPTED_ORDER);
        _tableViewAccepetedOrders->setModel(_acceptedOrders);

        _tableViewAccepetedOrders->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tableViewAccepetedOrders->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tableViewAccepetedOrders->verticalHeader()->hide();
        _tableViewAccepetedOrders->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        _parkedOrders = new ModelSingleOrder();
        _parkedOrders->setOrderToShow(ORDER_TO_SHOW::PARK_ORDER);
        _tableViewParkedOrders->setModel(_parkedOrders);

        _tableViewParkedOrders->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tableViewParkedOrders->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tableViewParkedOrders->verticalHeader()->hide();
        _tableViewParkedOrders->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        //设置列宽

    }

    //[3]
    {
        //_tabWidget->addTab(_tblNormalView, " 持 仓 ");
        //_tabWidget->addTab(_tblCompositeView, " 组合持仓 ");

        _tabWidget->addTab(_tableview, "已发订单");
        _tabWidget->addTab(_tableViewAccepetedOrders, "当前挂单");
       // _tabWidget->addTab(_tableViewParkedOrders, "预埋单");

        QHBoxLayout* h1 = new QHBoxLayout();
        h1->addWidget(_label);
        h1->addWidget(_combox);
        h1->addStretch();

        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addLayout(h1);
        mainLayout->addWidget(_tabWidget);

        setLayout(mainLayout);
    }

    //[4]
    QObject::connect(_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combox_indexChange(int)));
    if (_combox->count() > 0)
    {
        _combox->setCurrentIndex(0);
        on_combox_indexChange(0);
    }

    //[5]注册委托单通知
    QObject::connect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onOrderEvent(OrderEventArgs*)) );

    //[6]双击撤单
    QObject::connect(_tableview, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_tableview_doubleClicked(const QModelIndex&)) );
    QObject::connect(_tableViewAccepetedOrders, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_tableview_doubleClicked(const QModelIndex&)) );
    QObject::connect(_tableViewParkedOrders, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_tableview_doubleClicked(const QModelIndex&)) );

}

void MultiOrderWidget::closeEvent(QCloseEvent *event)
{
    QObject::disconnect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onOrderEvent(OrderEventArgs*)) );
/*
    if (_customModel)
    {
        _customModel->release();
        delete _customModel;
        _customModel = nullptr;
    }

    if(_acceptedOrders)
    {
        _acceptedOrders->release();
        delete _acceptedOrders;
        _acceptedOrders = nullptr;
    }
*/
    event->accept();
}

void   MultiOrderWidget::on_combox_indexChange(int)
{
    //int curindex = _combox->currentIndex();
    QVariant v = _combox->currentData();
    QOrderMgr* p = (QOrderMgr *)v.value<void *>();

    _customModel->setOrderMgr(p);
    _acceptedOrders->setOrderMgr(p);
  //  _parkedOrders->setOrderMgr(p);
}

void    MultiOrderWidget::onOrderEvent(OrderEventArgs* e)
{
    qDebug() << "MultiOrderWidget" << "onOrderEvent";

    //如果combox当前选择的账号，同通知的账号相同，则刷新
    QVariant v = _combox->currentData();
    QOrderMgr* p = (QOrderMgr *)v.value<void *>();
    if(p)
    {
        int usertype =0;
        int broker=0;
        char szuser[32]={0};
        p->WhoIs(usertype, broker, szuser);

        if(e->broker == broker &&
                e->usertype == usertype &&
                stricmp(e->user, szuser) == 0
          )
        {
            _customModel->setOrderMgr(p);
            _acceptedOrders->setOrderMgr(p);
        }
        if(e->broker == broker && stricmp(e->user, szuser) == 0)
        {
           //  _parkedOrders->setOrderMgr(p);
        }

    }
}

void    MultiOrderWidget::on_tableview_doubleClicked(const QModelIndex& index)
{
    qDebug() << "current active page id " << _tabWidget->currentIndex();
    tagXTOrderField* p = _tabWidget->currentIndex()==0 ?_customModel->getDataByRow(index): _acceptedOrders->getDataByRow(index);
    if(_tabWidget->currentIndex() == 0)
    {
        p = _customModel->getDataByRow(index);
    }
    if(_tabWidget->currentIndex() == 1)
    {
        p = _acceptedOrders->getDataByRow(index);
    }
    if(_tabWidget->currentIndex() == 2)
    {
     // reserve for parked order
    }
    if(p)
    {
        //如果状态为XT_OST_Accepted
        if(p->OrderStatus == XT_OST_Accepted)
        {
            //如果combox当前选择的账号，同通知的账号相同，则刷新
            QVariant v = _combox->currentData();
            QOrderMgr* _curMgr = (QOrderMgr *)v.value<void *>();
            if(_curMgr)
            {
                //弹出对话框确认
                QString strMsg;
                strMsg.sprintf("是否对委托单<font color='red'>[%d]</font>进行撤单?", p->OrderRef);
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

                    _curMgr->CancelOrder(&oField);
                }
            }

        }
    }
}
//============================================================================================================================================================================================================================
//============================================================================================================================================================================================================================
//============================================================================================================================================================================================================================
//============================================================================================================================================================================================================================
SingleOrderWidget::SingleOrderWidget(QWidget *parent /*= nullptr*/)
{
    setAttribute(Qt::WA_DeleteOnClose);

    initControls();

    _orderMrg = nullptr;
}

SingleOrderWidget::~SingleOrderWidget()
{

}

void    SingleOrderWidget::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);

    _customModel->setOrderMgr(_orderMrg);
}

void    SingleOrderWidget::initControls()
{
    //[0]
    _tableview = new QTableView();

    //[1]tableview
    {
        _customModel = new ModelSingleOrder();
        _tableview->setModel(_customModel);

        _tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tableview->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tableview->verticalHeader()->hide();
        _tableview->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        //设置列宽
    }

    //[2]
    {
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addWidget(_tableview);

        setLayout(mainLayout);
    }

    //[3]
    QObject::connect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onOrderEvent(OrderEventArgs*)) );

    //[6]双击撤单
    QObject::connect(_tableview, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_tableview_doubleClicked(const QModelIndex&)) );
}

void    SingleOrderWidget::closeEvent(QCloseEvent *event)
{
    QObject::disconnect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onOrderEvent(OrderEventArgs*)) );

    if (_customModel)
    {
        _customModel->release();
        delete _customModel;
        _customModel = nullptr;
    }

    event->accept();
}

void    SingleOrderWidget::onOrderEvent(OrderEventArgs* e)
{
    qDebug() << "SingleOrderWidget" << "onOrderEvent";

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
            _customModel->setOrderMgr(_orderMrg);
        }

    }
}

void    SingleOrderWidget::on_tableview_doubleClicked(const QModelIndex& index)
{
    tagXTOrderField* p = _customModel->getDataByRow(index);
    if(p)
    {
        //如果状态为XT_OST_Accepted
        if(p->OrderStatus == XT_OST_Accepted)
        {
            if(_orderMrg)
            {
                //弹出对话框确认
                QString strMsg;
                strMsg.sprintf("是否对委托单<font color='red'>[%d]</font>进行撤单?", p->BrokerOrderSeq);
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








