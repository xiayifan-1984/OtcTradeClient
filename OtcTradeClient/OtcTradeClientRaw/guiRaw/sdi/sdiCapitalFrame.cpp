
#include "sdiCapitalFrame.h"
#include "trademodule.h"
#include "docktitlebar.h"
#include <QDebug>

//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
#include "XTTradestruct.h"

class ModelSingleCapital :public QAbstractTableModel
{
public:
    ModelSingleCapital(QObject *parent = nullptr);

public:
    void        release();
    void        setOrderMgr(QOrderMgr* p);

public:
    int         rowCount(const QModelIndex &parent) const;
    int         columnCount(const QModelIndex &parent) const;

    QVariant    headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant    data(const QModelIndex &index, int role) const;


protected:
    QOrderMgr*                      m_pMgr;
    tagXTTradingAccountField        m_field;

    ///资金账户
    struct tagInnerTradingAccountField
    {
        ///经纪公司代码
        char	BrokerID[11];
        ///投资者帐号
        char	AccountID[13];
        ///上次质押金额
        double	PreMortgage;
        ///上次信用额度
        double	PreCredit;
        ///上次存款额
        double	PreDeposit;
        ///上次结算准备金
        double	PreBalance;

        double	PreMargin;          ///上次占用的保证金
        ///利息基数
        double	InterestBase;
        ///利息收入
        double	Interest;

        double	Deposit;            ///入金金额

        double	Withdraw;           ///出金金额

        double	FrozenMargin;       ///冻结的保证金
        ///冻结的资金
        double	FrozenCash;

        double	FrozenCommission;   ///冻结的手续费

        double	CurrMargin;         ///当前保证金总额
        ///资金差额
        double	CashIn;

        double	Commission;         ///手续费

        double	CloseProfit;        ///平仓盈亏

        double	PositionProfit;     ///持仓盈亏
        ///期货结算准备金
        double	Balance;

        double	Available;          ///可用资金
        ///可取资金
        double	WithdrawQuota;
        ///基本准备金
        double	Reserve;
        ///交易日
        char	TradingDay[9];
        ///结算编号
        int     SettlementID;
        ///信用额度
        double	Credit;
        ///质押金额
        double	Mortgage;
        ///交易所保证金
        double	ExchangeMargin;
        ///投资者交割保证金
        double	DeliveryMargin;
        ///交易所交割保证金
        double	ExchangeDeliveryMargin;
        ///保底期货结算准备金
        double	ReserveBalance;
        ///币种代码
        char	CurrencyID[4];
        ///上次货币质入金额
        double	PreFundMortgageIn;
        ///上次货币质出金额
        double	PreFundMortgageOut;
        ///货币质入金额
        double	FundMortgageIn;
        ///货币质出金额
        double	FundMortgageOut;
        ///货币质押余额
        double	FundMortgageAvailable;
        ///可质押货币金额
        double	MortgageableFund;
        ///特殊产品占用保证金
        double	SpecProductMargin;
        ///特殊产品冻结保证金
        double	SpecProductFrozenMargin;
        ///特殊产品手续费
        double	SpecProductCommission;
        ///特殊产品冻结手续费
        double	SpecProductFrozenCommission;
        ///特殊产品持仓盈亏
        double	SpecProductPositionProfit;
        ///特殊产品平仓盈亏
        double	SpecProductCloseProfit;
        ///根据持仓盈亏算法计算的特殊产品持仓盈亏
        double	SpecProductPositionProfitByAlg;
        ///特殊产品交易所保证金
        double	SpecProductExchangeMargin;
        ///业务类型
        char	BizType;
        ///延时换汇冻结金额
        double	FrozenSwap;
        ///剩余换汇额度
        double	RemainSwap;
    };


};

//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
#define CURRENT_COL_COUNT (10)


ModelSingleCapital::ModelSingleCapital(QObject *parent /*= nullptr*/)
{
    m_pMgr = nullptr;
    memset(&m_field, 0, sizeof(m_field) );
}


void        ModelSingleCapital::release()
{

}

void        ModelSingleCapital::setOrderMgr(QOrderMgr* p)
{
    beginResetModel();

    release();
    m_pMgr = p;

    if(p)
    {

        p->GetTradingAccount(&m_field);
    }

    //刷新所有视图数据
    endResetModel();
}

int         ModelSingleCapital::rowCount(const QModelIndex &parent) const
{
    return 1;
}

int         ModelSingleCapital::columnCount(const QModelIndex &parent) const
{
    return  CURRENT_COL_COUNT;
}

QVariant    ModelSingleCapital::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Vertical)
    {
        QString str = QString("%1").arg(section);
        return str;
    }
    if (orientation == Qt::Horizontal)
    {
        QString str = "";
        switch (section)
        {
        case 0:str = tr("静态权益"); break;        //上次占用的保证金 PreMargin
        case 1:str = tr("入金"); break;           //入金Deposit
        case 2:str = tr("出金"); break;           //出金Withdraw
        case 3:str = tr("平仓盈亏"); break;         //平仓盈亏CloseProfit
        case 4:str = tr("持仓盈亏");break;            //持仓盈亏PositionProfit

        case 5:str = tr("手续费");break;           //手续费Commission
        case 6:str = tr("保证金");break;           //当前保证金总额CurrMargin
        case 7:str = tr("冻结保证金");break;         //冻结的保证金FrozenMargin
        case 8:str = tr("冻结手续费");break;         //冻结的手续费FrozenCommission
        case 9:str= tr("可用资金");break;           //可用资金Available
        }
        return str;
    }

    return QVariant();
}

QVariant    ModelSingleCapital::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole)  //文字排列
    {
        int align = int(Qt::AlignRight | Qt::AlignVCenter);
        int col = index.column();
        int row = index.row();

        return align;
    }
    else if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();

        if (m_field.OriSize ==0)
        {
            return  QVariant();
        }
        if (row != 0)
        {
            return  QVariant();
        }

        if (col < 0 || col >= CURRENT_COL_COUNT )
        {
            return QVariant();
        }

        QString str = "";
        tagInnerTradingAccountField* p = (tagInnerTradingAccountField*)(m_field._reserved);

        switch (col)
        {
        case 0:str.sprintf("%.04lf", p->PreMargin); break;//上次占用的保证金
        case 1:str.sprintf("%.04lf", p->Deposit); break;//入金

        case 2:str.sprintf("%.04lf", p->Withdraw); break;//出金
        case 3:str.sprintf("%.04lf", p->CloseProfit); break;//平仓盈亏
        case 4:str.sprintf("%.04lf", p->PositionProfit); break;//持仓盈亏
        case 5:str.sprintf("%.04lf", p->Commission); break;//手续费
        case 6:str.sprintf("%.04lf", p->CurrMargin); break;//当前保证金总额
        case 7:str.sprintf("%.04lf", p->FrozenMargin); break;//冻结的保证金
        case 8:str.sprintf("%.04lf", p->FrozenCommission); break;//冻结的手续费
        case 9:str.sprintf("%.04lf", p->Available); break;//可用资金

        }
        return str;
    }

    return QVariant();
}

//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
CSDICapitalFrame::CSDICapitalFrame(QWidget *parent):
    QFrame(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    _tblViewNormal = nullptr;
    _modelNormal = nullptr;
    _btnQuery = nullptr;
    _orderMrg = nullptr;
    _titlebar = nullptr;

    _timer = nullptr;

}

CSDICapitalFrame::~CSDICapitalFrame()
{

}

void   CSDICapitalFrame::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);
    if (_modelNormal)
    {
        _modelNormal->setOrderMgr(_orderMrg);
    }
}

void   CSDICapitalFrame::setUserAccount(QOrderMgr* p)
{
    _orderMrg = p;
    if (_modelNormal)
    {
        _modelNormal->setOrderMgr(_orderMrg);
    }
}

void   CSDICapitalFrame::initControls()
{
    //[1]
    _btnQuery = new QPushButton(tr("查询"));
    QObject::connect(_btnQuery, SIGNAL(clicked()), this, SLOT(onQueryBtnClick()) );

    _tblViewNormal = new QTableView();
    _tblViewNormal->setStyleSheet("QHeaderView{border:0px solid rgb(145,145,145)}");

    //[2]
    {
        _modelNormal = new ModelSingleCapital();
        _tblViewNormal->setModel(_modelNormal);

        _tblViewNormal->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tblViewNormal->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tblViewNormal->verticalHeader()->hide();  //隐藏标号
        _tblViewNormal->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        //设置列宽
        for(int i = 0; i < 10; i++)
        {
            _tblViewNormal->setColumnWidth(i, 120);
        }

    }

    //[4]创建Titlebar
    {
        _titlebar = new CDockTitlebar();
        _titlebar->setTitle(tr("资金信息"));
        _titlebar->setReady();
    }

    //[5]布局
    {
        QHBoxLayout* subLayout = new QHBoxLayout();
        subLayout->addWidget(_tblViewNormal);
        subLayout->addWidget(_btnQuery);
        subLayout->setSpacing(10);      //元素之间， 空10个像素
        subLayout->addSpacing(10);      //最后一个元素，再添加10个像素

        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addWidget(_titlebar);
        mainLayout->addLayout(subLayout);
        this->setLayout(mainLayout);
        mainLayout->setSpacing(0);
        mainLayout->setMargin(2);
    }

    _timer = new QTimer(this);
}

void CSDICapitalFrame::closeEvent(QCloseEvent *event)
{
    if (_modelNormal)
    {
        _modelNormal->release();
        delete _modelNormal;
        _modelNormal = nullptr;
    }

    if(_timer)
    {
        delete _timer;
        _timer = nullptr;
    }

    event->accept();
}

void CSDICapitalFrame::onQueryBtnClick()
{
    if(nullptr == _orderMrg)
    {
        return;
    }

    _orderMrg->ReqQryAccount();

    if(_timer)
    {
        _timer->singleShot(1500, this, SLOT(onTimer()) );
    }

}

void    CSDICapitalFrame::onTimer()
{
    setUserAccount(_orderMrg);
}









