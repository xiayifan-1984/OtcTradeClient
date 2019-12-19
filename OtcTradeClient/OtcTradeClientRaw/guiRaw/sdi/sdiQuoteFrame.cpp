
#include "sdiQuoteFrame.h"
#include "trademodule.h"
#include "quotemodule.h"
#include "docktitlebar.h"
#include <QDebug>
#include <vector>

//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
//.........................................................................................................................................................................................

class ModelSingleQuote :public QAbstractTableModel
{
public:
    ModelSingleQuote(QObject *parent = nullptr);

public:
    void        release();

    void        setQuoteData();

public:
    int         rowCount(const QModelIndex &parent) const;
    int         columnCount(const QModelIndex &parent) const;

    QVariant    headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant    data(const QModelIndex &index, int role) const;

    QColor      getColorByPrice(double preprice, double price) const;

protected:
   std::vector<tagXTInstrument>      m_arrCodeTable;
   std::vector<tagXTSnapshot>        m_arrSnapTable;

};

//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
#define COL_COUNT  (19)
ModelSingleQuote::ModelSingleQuote(QObject *parent)
{

}

void        ModelSingleQuote::release()
{
    m_arrCodeTable.clear();
    m_arrSnapTable.clear();
}

void        ModelSingleQuote::setQuoteData()
{
    beginResetModel();

    release();

    GetQuoteModule()->GetRegisterCodeList(m_arrCodeTable);
    int nCount = m_arrCodeTable.size();
    if(nCount >0)
    {
        for(int i=0; i< nCount; i++)
        {
            tagXTSnapshot oSnap;
            memset(&oSnap, 0, sizeof(oSnap));
            GetQuoteModule()->GetSnapShot(m_arrCodeTable[i], &oSnap);
            m_arrSnapTable.push_back(oSnap);
        }

    }

    //刷新所有视图数据
    endResetModel();
}

int         ModelSingleQuote::rowCount(const QModelIndex &parent) const
{
    return m_arrCodeTable.size();
}

int         ModelSingleQuote::columnCount(const QModelIndex &parent) const
{
    return COL_COUNT;
}

QVariant    ModelSingleQuote::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Vertical)
    {
        QString str = QString("%1").arg(section);
        return str;
    }
    if(orientation == Qt::Horizontal)
    {
        QString str = "";
        switch(section)
        {
        case 0:str = "时间";break;
        case 1:str = "合约";break;
        case 2:str = "最新价"; break;
        case 3:str = "买价"; break;
        case 4:str = "买量"; break;
        case 5:str = "卖价"; break;
        case 6:str = "卖量"; break;
        case 7:str = "跌停价"; break;
        case 8:str = "涨停价"; break;
        case 9:str = "今开盘"; break;
        case 10:str = "昨收盘"; break;
        case 11:str = "昨结算"; break;
        case 12:str = "成交量"; break;
        case 13:str = "持仓量"; break;
        case 14:str = "昨持仓量"; break;
        case 15:str = "最高价"; break;
        case 16:str = "今收盘"; break;
        case 17:str = "结算价"; break;
        case 18:str = "成交额"; break;
        }
        return str;
    }

}

QColor      ModelSingleQuote::getColorByPrice(double preprice, double price) const
{
    if(price > preprice)
    {
        return QColor(206,66,66);  //红色
    }
    else if(price < preprice)
    {
        return QColor(66,206,66);  //绿色
    }
    else
    {
        //return QColor(255,255,255);
        return QColor(0, 102, 214); //蓝色
    }
}

QVariant    ModelSingleQuote::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    else if(role == Qt::TextColorRole)   //文字颜色
    {
        int row = index.row();
        int col = index.column();
        if(row >=0 && row < m_arrSnapTable.size() )
        {
            if(m_arrSnapTable[row].LastPrice >0)
            {
                if(col == 2)
                    return getColorByPrice(m_arrSnapTable[row].PreClosePrice, m_arrSnapTable[row].LastPrice );
                else if(col == 3)
                    return getColorByPrice(m_arrSnapTable[row].PreClosePrice,m_arrSnapTable[row].BuyItem[0].Price);
                else if(col == 5)
                    return getColorByPrice(m_arrSnapTable[row].PreClosePrice,m_arrSnapTable[row].SellItem[0].Price);
            }

        }
    }
    else if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();

        if(row <0 || row >= m_arrSnapTable.size())
        {
            return  QVariant();
        }

        if(col <0 || col >= COL_COUNT)
        {
            return QVariant();
        }

        QString str = "";

        switch(col)
        {
        case 0:
        {
            int msec = m_arrSnapTable[row].UpdateTime%1000;
            int ntime = m_arrSnapTable[row].UpdateTime/1000;
            str.sprintf("%02d:%02d:%02d.%03d" , ntime/10000, (ntime%10000)/100, ntime%100 , msec);
        }
            break;
        case 1:str = m_arrSnapTable[row].ExCode.Code; break;
        case 2:str.sprintf("%.01lf", m_arrSnapTable[row].LastPrice);break;
        case 3:str.sprintf("%.01lf", m_arrSnapTable[row].BuyItem[0].Price);break;
        case 4:str.sprintf("%d", m_arrSnapTable[row].BuyItem[0].Volume);break;
        case 5:str.sprintf("%.01lf", m_arrSnapTable[row].SellItem[0].Price);break;
        case 6:str.sprintf("%d", m_arrSnapTable[row].SellItem[0].Volume);break;
        case 7:str.sprintf("%.01lf", m_arrSnapTable[row].LowerLimitPrice);break;
        case 8:str.sprintf("%.01lf", m_arrSnapTable[row].UpperLimitPrice);break;
        case 9:str.sprintf("%.01lf", m_arrSnapTable[row].OpenPrice);break;
        case 10:str.sprintf("%.01lf", m_arrSnapTable[row].PreClosePrice);break;
        case 11:str.sprintf("%.01lf", m_arrSnapTable[row].PreSettlementPrice);break;
        case 12:str.sprintf("%d",m_arrSnapTable[row].Volume);break;
        case 13:str.sprintf("%d", m_arrSnapTable[row].OpenInterest);break;
        case 14:str.sprintf("%d", m_arrSnapTable[row].PreOpenInterest);break;
        case 15:str.sprintf("%.01lf",m_arrSnapTable[row].HighestPrice);break;
        case 16:str.sprintf("%.01lf",m_arrSnapTable[row].ClosePrice);break;
        case 17:str.sprintf("%.01lf",m_arrSnapTable[row].SettlementPrice);break;
        case 18:str.sprintf("%.01lf",m_arrSnapTable[row].Turnover);break;
        }

        return str;
    }
    return QVariant();

}

//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
//.........................................................................................................................................................................................
CSDIQuoteFrame::CSDIQuoteFrame(QWidget *parent):
    QFrame(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    _tblViewNormal = nullptr;
    _modelNormal = nullptr;

    _orderMrg = nullptr;
    _titlebar = nullptr;
    _timer = nullptr;
}

CSDIQuoteFrame::~CSDIQuoteFrame()
{

}

void   CSDIQuoteFrame::setUserAccount(int usertype, int broker, char* userid)
{
    _orderMrg = GetTradeModule()->GetMgr(usertype, broker, userid);
}

void   CSDIQuoteFrame::setUserAccount(QOrderMgr* p)
{
    _orderMrg = p;
}

void   CSDIQuoteFrame::initControls()
{
    //[1]
    _tblViewNormal = new QTableView();
    _tblViewNormal->setStyleSheet("QHeaderView{border:0px solid rgb(145,145,145)}");

    //[2]
    {
        _modelNormal = new ModelSingleQuote();
        _tblViewNormal->setModel(_modelNormal);

        _tblViewNormal->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        _tblViewNormal->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        _tblViewNormal->verticalHeader()->hide();  //隐藏标号
        _tblViewNormal->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

        //设置列宽
        for(int i = 0; i < COL_COUNT; i++)
        {
            _tblViewNormal->setColumnWidth(i, 120);
        }

    }

    //[4]创建Titlebar
    {
        _titlebar = new CDockTitlebar();
        _titlebar->setTitle(tr("行情数据"));
        _titlebar->setReady();
    }

    //[5]布局
    {
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addWidget(_titlebar);
        mainLayout->addWidget(_tblViewNormal);
        this->setLayout(mainLayout);
        mainLayout->setSpacing(0);
        mainLayout->setMargin(2);
    }

    _timer = new QTimer(this);
    QObject::connect(_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    _timer->start(500);

}

void CSDIQuoteFrame::closeEvent(QCloseEvent *event)
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

void    CSDIQuoteFrame::onTimer()
{
    if (_modelNormal)
    {
        _modelNormal->setQuoteData();
    }
}




























