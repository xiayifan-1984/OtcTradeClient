#include "datatrade.h"
#include "configmodule.h"
#include "trademodule.h"
#include "XTCodec.h"
#include "eventcentermodule.h"
#include "modeldatatrade.h"

//============================================================================================================================================================================================================================
MultiTradeWidget::MultiTradeWidget(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    initControls();

}

MultiTradeWidget::~MultiTradeWidget()
{

}

void    MultiTradeWidget::initControls()
{
    //[0]
    _label = new QLabel();
    _combox = new QComboBox();
    _tableview = new QTableView();

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
        _customModel = new ModelSingleTrade();
        _tableview->setModel(_customModel);

        _tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tableview->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tableview->verticalHeader()->hide();  //隐藏标号
        _tableview->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        //设置列宽
        _tableview->setColumnWidth(0, 80);
        _tableview->setColumnWidth(1, 80);
        _tableview->setColumnWidth(2, 80);
        _tableview->setColumnWidth(3, 80);

        _tableview->setColumnWidth(5, 80);
        _tableview->setColumnWidth(6, 80);
        _tableview->setColumnWidth(9, 80);
    }

    //[3]
    {
        QHBoxLayout* h1 = new QHBoxLayout();
        h1->addWidget(_label);
        h1->addWidget(_combox);
        h1->addStretch();

        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addLayout(h1);
        mainLayout->addWidget(_tableview);

        setLayout(mainLayout);
    }

    //[4]
    QObject::connect(_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combox_indexChange(int)));
    if (_combox->count() > 0)
    {
        _combox->setCurrentIndex(0);
        on_combox_indexChange(0);
    }

    //[5]
    QObject::connect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );

}

void MultiTradeWidget::closeEvent(QCloseEvent *event)
{
    QObject::disconnect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );

    if (_customModel)
    {
        _customModel->release();
        delete _customModel;
        _customModel = nullptr;
    }

    event->accept();
}

void   MultiTradeWidget::on_combox_indexChange(int)
{
    //int curindex = _combox->currentIndex();
    QVariant v = _combox->currentData();
    QOrderMgr* p = (QOrderMgr *)v.value<void *>();

    _customModel->setOrderMgr(p);
}

void   MultiTradeWidget::onTransactEvent(TransactionEventArgs* e)
{
    qDebug() << "MultiTradeWidget" << "onTransactEvent";

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
        }

    }
}


//============================================================================================================================================================================================================================
//============================================================================================================================================================================================================================
//============================================================================================================================================================================================================================
//============================================================================================================================================================================================================================
SingleTradeWidget::SingleTradeWidget(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    initControls();

    _orderMrg = nullptr;
}

SingleTradeWidget::~SingleTradeWidget()
{

}

void    SingleTradeWidget::initControls()
{
    //[0]
    _tableview = new QTableView();

    //[1]tableview
    {
        _customModel = new ModelSingleTrade();
        _tableview->setModel(_customModel);

        _tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tableview->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tableview->verticalHeader()->hide();  //隐藏标号
        _tableview->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        //设置列宽
        _tableview->setColumnWidth(0, 80);
        _tableview->setColumnWidth(1, 80);
        _tableview->setColumnWidth(2, 80);
        _tableview->setColumnWidth(3, 80);

        _tableview->setColumnWidth(5, 80);
        _tableview->setColumnWidth(6, 80);
        _tableview->setColumnWidth(9, 80);
    }

    //[2]
    {
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addWidget(_tableview);

        setLayout(mainLayout);
    }

    //[5]
    QObject::connect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );

}

void SingleTradeWidget::closeEvent(QCloseEvent *event)
{
    QObject::disconnect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );

    if (_customModel)
    {
        _customModel->release();
        delete _customModel;
        _customModel = nullptr;
    }

    event->accept();
}

void   SingleTradeWidget::onTransactEvent(TransactionEventArgs* e)
{
    qDebug() << "SingleTradeWidget" << "onTransactEvent";

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

void    SingleTradeWidget::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);

    _customModel->setOrderMgr(_orderMrg);
}


