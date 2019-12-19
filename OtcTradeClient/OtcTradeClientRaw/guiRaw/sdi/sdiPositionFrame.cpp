
#include "sdiPositionFrame.h"
#include "configmodule.h"
#include "trademodule.h"
#include "XTCodec.h"
#include "eventcentermodule.h"
#include "modeldataposition.h"
#include "docktitlebar.h"
#include "positionTable.h"
#include <QDebug>


CSDIPositionFrame::CSDIPositionFrame(QWidget* parent ):
    QFrame(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

	_orderMrg = nullptr;
	_titlebar = nullptr;
    _tabWidget = nullptr;
    _tblViewNormal = nullptr;
    _tblViewComposite = nullptr;
    _modelNormalPosition = nullptr;
    _modelCompositePosition = nullptr;

    _tblViewContract = nullptr;
    _tblViewLog = nullptr;
}

CSDIPositionFrame::~CSDIPositionFrame()
{

}

void        CSDIPositionFrame::setUserAccount(int usertype, int broker, char*  userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);

    if(_modelNormalPosition)
    {
        _modelNormalPosition->setOrderMgr(_orderMrg);
    }
    if(_modelCompositePosition)
    {
        _modelCompositePosition->setOrderMgr(_orderMrg);
    }
}

void        CSDIPositionFrame::setUserAccount(QOrderMgr* p)
{
    _orderMrg = p;

    if(_modelNormalPosition)
    {
        _modelNormalPosition->setOrderMgr(_orderMrg);
    }
    if(_modelCompositePosition)
    {
        _modelCompositePosition->setOrderMgr(_orderMrg);
    }
}


void        CSDIPositionFrame::initControls()
{
    //[0]
    _tabWidget = new QTabWidget();
    _tabWidget->setTabPosition(QTabWidget::South);
    _tabWidget->setStyleSheet("border:0px solid rgb(145,145,145)");

    _tblViewNormal = new QTableView();
    _tblViewComposite = new QTableView();

    //[1]tableview
    {
        _modelNormalPosition = new ModelNormalPosition();
        _tblViewNormal->setModel(_modelNormalPosition);

        _tblViewNormal->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tblViewNormal->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tblViewNormal->verticalHeader()->hide();
        _tblViewNormal->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        //设置列宽
        _tblViewNormal->setColumnWidth(0, 160);
        _tblViewNormal->setColumnWidth(1, 80);
        _tblViewNormal->setColumnWidth(2, 80);
        _tblViewNormal->setColumnWidth(3, 100);
        _tblViewNormal->setColumnWidth(4, 80);
        _tblViewNormal->setColumnWidth(5, 80);
        _tblViewNormal->setColumnWidth(6, 80);
        _tblViewNormal->setColumnWidth(7, 80);

    }
    //[2]
    {
        _modelCompositePosition = new ModelCompositePosition();
        _tblViewComposite->setModel(_modelCompositePosition);

        _tblViewComposite->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tblViewComposite->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tblViewComposite->verticalHeader()->hide();
        _tblViewComposite->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        //设置列宽
        _tblViewComposite->setColumnWidth(0, 160);
        _tblViewComposite->setColumnWidth(1, 80);
        _tblViewComposite->setColumnWidth(2, 80);
        _tblViewComposite->setColumnWidth(3, 80);
        _tblViewComposite->setColumnWidth(4, 80);

    }

    {
        _tblViewContract = new CContractTableView();
        _tblViewLog = new CLogTableView();
    }

    //[3]创建TabWidget
    {
        _tabWidget->addTab(_tblViewNormal, "持仓信息 ( F4 ) ");
        _tabWidget->addTab(_tblViewComposite, "组合持仓 ( F7 ) ");
        _tabWidget->addTab(_tblViewContract, "合约列表 ( F8 ) ");
        _tabWidget->addTab(_tblViewLog, " 信 息 日 志 ");
    }

    //[4]创建Titlebar
    {
        _titlebar = new CDockTitlebar();
        _titlebar->addStockID(TITLEBUTTON_CONFIG);
        _titlebar->addStockID(TITLEBUTTON_MENU);
        _titlebar->setTitle(tr("持仓信息"));
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
    QObject::connect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );
    QObject::connect( _tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabSelectChanged(int))  );

}


void        CSDIPositionFrame::closeEvent(QCloseEvent *event)
{
    QObject::disconnect( GetEventCenter(), SIGNAL(fireTransactEvent(TransactionEventArgs*)), this, SLOT(onTransactEvent(TransactionEventArgs*))  );

    if (_modelNormalPosition)
    {
        _modelNormalPosition->release();
        delete _modelNormalPosition;
        _modelNormalPosition = nullptr;
    }
    if (_modelCompositePosition)
    {
        _modelCompositePosition->release();
        delete _modelCompositePosition;
        _modelCompositePosition = nullptr;
    }

    event->accept();
}


void        CSDIPositionFrame::onTransactEvent(TransactionEventArgs* e)
{
    qDebug() << "CSDIPositionFrame" << "onTransactEvent";

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
            _modelNormalPosition->setOrderMgr(_orderMrg);
            _modelCompositePosition->setOrderMgr(_orderMrg);
        }

    }
}

void    CSDIPositionFrame::onTabSelectChanged(int index)
{
    qDebug() << "CSDIPositionFrame::onTabSelectChanged" << index;

    if(_titlebar == nullptr)
    {
        return;
    }

    if(index == 0)
    {
        _titlebar->setTitle(tr("持仓信息"));
    }
    else if(index ==1)
    {
        _titlebar->setTitle(tr("组合持仓"));
    }
    else if(index == 2)
    {
        _titlebar->setTitle(tr("合约列表"));
    }
    else if(index == 3)
    {
        _titlebar->setTitle(tr("信息日志"));
    }
}




