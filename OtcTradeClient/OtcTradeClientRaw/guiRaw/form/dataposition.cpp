#include "dataposition.h"
#include "configmodule.h"
#include "trademodule.h"
#include "XTCodec.h"
#include "eventcentermodule.h"
#include "modeldataposition.h"

//============================================================================================================================================================================================================================
MultiPositionWidget::MultiPositionWidget(QWidget *parent) :
	QWidget(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);

	initControls();

}

MultiPositionWidget::~MultiPositionWidget()
{

}

void    MultiPositionWidget::initControls()
{
	//[0]
	_label = new QLabel();
	_combox = new QComboBox();
    _tabWidget = new QTabWidget();

    _tblNormalView = new QTableView();
    _tblCompositeView = new QTableView();

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
        _csmNormalModel = new ModelNormalPosition();
        _tblNormalView->setModel(_csmNormalModel);

        _tblNormalView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tblNormalView->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tblNormalView->verticalHeader()->hide();
        _tblNormalView->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

		//设置列宽
        _tblNormalView->setColumnWidth(0, 160);
        _tblNormalView->setColumnWidth(1, 80);
        _tblNormalView->setColumnWidth(2, 80);
        _tblNormalView->setColumnWidth(3, 80);

        _tblNormalView->setColumnWidth(7, 80);
        _tblNormalView->setColumnWidth(8, 80);

	}
    //[3]
    {
        _csmCompositeModel = new ModelCompositePosition();
        _tblCompositeView->setModel(_csmCompositeModel);

        _tblCompositeView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tblCompositeView->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tblCompositeView->verticalHeader()->hide();
        _tblCompositeView->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        //设置列宽
        _tblCompositeView->setColumnWidth(0, 160);
        _tblCompositeView->setColumnWidth(1, 80);
        _tblCompositeView->setColumnWidth(2, 80);
        _tblCompositeView->setColumnWidth(3, 80);
        _tblCompositeView->setColumnWidth(4, 80);

    }

    //[4]
	{
        _tabWidget->addTab(_tblNormalView, " 持 仓 ");
        _tabWidget->addTab(_tblCompositeView, " 组合持仓 ");

		QHBoxLayout* h1 = new QHBoxLayout();
		h1->addWidget(_label);
		h1->addWidget(_combox);
		h1->addStretch();

		QVBoxLayout* mainLayout = new QVBoxLayout();
		mainLayout->addLayout(h1);
        mainLayout->addWidget(_tabWidget);

		setLayout(mainLayout);
	}

    //[5]
	QObject::connect(_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combox_indexChange(int)));
	if (_combox->count() > 0)
	{
		_combox->setCurrentIndex(0);
		on_combox_indexChange(0);
	}

    //[6]
    QObject::connect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );
}

void MultiPositionWidget::closeEvent(QCloseEvent *event)
{
    QObject::disconnect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );

    if (_csmNormalModel)
	{
        _csmNormalModel->release();
        delete _csmNormalModel;
        _csmNormalModel = nullptr;
	}
    if (_csmCompositeModel)
    {
        _csmCompositeModel->release();
        delete _csmCompositeModel;
        _csmCompositeModel = nullptr;
    }

	event->accept();
}

void   MultiPositionWidget::on_combox_indexChange(int)
{
	//int curindex = _combox->currentIndex();
	QVariant v = _combox->currentData();
	QOrderMgr* p = (QOrderMgr *)v.value<void *>();

    _csmNormalModel->setOrderMgr(p);
    _csmCompositeModel->setOrderMgr(p);
}

void   MultiPositionWidget::onTransactEvent(TransactionEventArgs* e)
{
    qDebug() << "MultiPositionWidget" << "onTransactEvent";

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
            _csmNormalModel->setOrderMgr(p);
            _csmCompositeModel->setOrderMgr(p);
        }

    }
}


//============================================================================================================================================================================================================================
SinglePositionWidget::SinglePositionWidget(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    initControls();

    _orderMrg = nullptr;
}

SinglePositionWidget::~SinglePositionWidget()
{

}

void    SinglePositionWidget::initControls()
{
    //[0]
    _tabWidget = new QTabWidget();

    _tblNormalView = new QTableView();
    _tblCompositeView = new QTableView();

    //[1]tableview
    {
        _csmNormalModel = new ModelNormalPosition();
        _tblNormalView->setModel(_csmNormalModel);

        _tblNormalView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tblNormalView->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tblNormalView->verticalHeader()->hide();
        _tblNormalView->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        //设置列宽
        _tblNormalView->setColumnWidth(0, 160);
        _tblNormalView->setColumnWidth(1, 80);
        _tblNormalView->setColumnWidth(2, 80);
        _tblNormalView->setColumnWidth(3, 80);

        _tblNormalView->setColumnWidth(7, 80);
        _tblNormalView->setColumnWidth(8, 80);

    }
    //[2]
    {
        _csmCompositeModel = new ModelCompositePosition();
        _tblCompositeView->setModel(_csmCompositeModel);

        _tblCompositeView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tblCompositeView->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tblCompositeView->verticalHeader()->hide();
        _tblCompositeView->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        //设置列宽
        _tblCompositeView->setColumnWidth(0, 160);
        _tblCompositeView->setColumnWidth(1, 80);
        _tblCompositeView->setColumnWidth(2, 80);
        _tblCompositeView->setColumnWidth(3, 80);
        _tblCompositeView->setColumnWidth(4, 80);

    }

    //[3]
    {
        _tabWidget->addTab(_tblNormalView, " 持 仓 ");
        _tabWidget->addTab(_tblCompositeView, " 组合持仓 ");

        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addWidget(_tabWidget);
        setLayout(mainLayout);
    }

    //[4]
    QObject::connect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );
}

void SinglePositionWidget::closeEvent(QCloseEvent *event)
{
    QObject::disconnect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );

    if (_csmNormalModel)
    {
        _csmNormalModel->release();
        delete _csmNormalModel;
        _csmNormalModel = nullptr;
    }
    if (_csmCompositeModel)
    {
        _csmCompositeModel->release();
        delete _csmCompositeModel;
        _csmCompositeModel = nullptr;
    }

    event->accept();
}

void   SinglePositionWidget::onTransactEvent(TransactionEventArgs* e)
{
    qDebug() << "SinglePositionWidget" << "onTransactEvent";

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
            _csmNormalModel->setOrderMgr(_orderMrg);
            _csmCompositeModel->setOrderMgr(_orderMrg);
        }

    }
}

void    SinglePositionWidget::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);

    _csmNormalModel->setOrderMgr(_orderMrg);
    _csmCompositeModel->setOrderMgr(_orderMrg);
}



