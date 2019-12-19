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

    //[1]
    {
        _label->setText(tr("Account:"));

        _combox->setEditable(false);
        _combox->setView(new QListView());

        vector<tagTTradeUserParam> arrUser;
        GetConfigModule()->GetAllTradeUser(arrUser);
        int ncount = arrUser.size();
        for (int i = 0; i < ncount; i++)
        {
            tagTTradeUserParam& o = arrUser[i];

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
        _acceptedOrders->setOrderToShow(ModelSingleOrder::ACCEPTED_ORDER);
        _tableViewAccepetedOrders->setModel(_acceptedOrders);

        _tableViewAccepetedOrders->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tableViewAccepetedOrders->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tableViewAccepetedOrders->verticalHeader()->hide();
        _tableViewAccepetedOrders->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

    }

    //[3]
    {
        _tabWidget->addTab(_tableview, "已发订单");
        _tabWidget->addTab(_tableViewAccepetedOrders, "当前挂单");

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

}

void MultiOrderWidget::closeEvent(QCloseEvent *event)
{
    QObject::disconnect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onOrderEvent(OrderEventArgs*)) );

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

    event->accept();
}

void   MultiOrderWidget::on_combox_indexChange(int)
{
    QVariant v = _combox->currentData();
    QOrderMgr* p = (QOrderMgr *)v.value<void *>();

    _customModel->setOrderMgr(p);
    _acceptedOrders->setOrderMgr(p);
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

    }
}

void    MultiOrderWidget::on_tableview_doubleClicked(const QModelIndex& index)
{
    tagXTOrderField* p = nullptr;
    if(_tabWidget->currentIndex() == 0)
    {
        p = _customModel->getDataByRow(index);
    }
    if(_tabWidget->currentIndex() == 1)
    {
        p = _acceptedOrders->getDataByRow(index);
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







