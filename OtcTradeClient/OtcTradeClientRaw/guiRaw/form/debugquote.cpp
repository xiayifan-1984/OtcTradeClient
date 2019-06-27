#include "debugquote.h"
#include "ui_debugquote.h"
#include "datamodule.h"
#include "XTCodec.h"

#include "quotemodule.h"
#include <map>
using namespace std;

//============================================================================================================================================================================================================================
class QuoteDataModel : public QAbstractTableModel
{
public:
    QuoteDataModel(QObject *parent = nullptr)
    {
        m_pCodeTable = nullptr;
        m_pSnapTable = nullptr;
        m_nDataSize =0;
        m_bBlank = true;
    }

    void        release()
    {
        if(m_pCodeTable)
        {
            delete []m_pCodeTable;
            m_pCodeTable =nullptr;
        }
        if(m_pSnapTable)
        {
            delete []m_pSnapTable;
            m_pSnapTable =nullptr;
        }
        m_nDataSize =0;

    }

    void        setQuoteData(QExchange* p, int productID)
    {
        beginResetModel();

        release();

        if(p)
        {
            int ncount = p->GetCodeListByProduct(nullptr, 0, productID);

            if(ncount >0)
            {
                m_pCodeTable = new tagXTInstrument[ncount];
                p->GetCodeListByProduct(m_pCodeTable, ncount, productID);

                m_pSnapTable = new tagXTSnapshot[ncount];
                for(int i=0; i<ncount; i++)
                {
                    int iret = GetQuoteModule()->GetSnapShot(m_pCodeTable[i], &m_pSnapTable[i]);
                    if(iret <= 0)
                    {
                        memset(&m_pSnapTable[i], 0, sizeof(tagXTSnapshot));
                    }
                }
            }
            m_nDataSize = ncount;

            m_bBlank = (ncount >0)?false:true;
        }
        else
        {
            m_bBlank = true;
        }

        //刷新所有视图数据
        endResetModel();
    }

    void        doreset()
    {
        if(!m_bBlank)
        {
            //刷新所有视图数据
            beginResetModel();
            if(m_nDataSize >0)
            {
                for(int i=0; i<m_nDataSize; i++)
                {
                    int iret = GetQuoteModule()->GetSnapShot(m_pCodeTable[i], &m_pSnapTable[i]);
                    if(iret <= 0)
                    {
                        memset(&m_pSnapTable[i], 0, sizeof(tagXTSnapshot));
                    }
                }
            }
            endResetModel();
        }

    }

    int         rowCount(const QModelIndex &parent) const
    {
        return m_nDataSize;
    }
    int         columnCount(const QModelIndex &parent) const
    {
        return 11;
    }

    QColor      getColorByPrice(double preprice, double price) const
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
    QVariant    data(const QModelIndex &index, int role) const
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
            if(row >=0 && row < m_nDataSize)
            {
                if(m_pSnapTable[row].ExCode.ExchID >0)
                {
                    if(col == 2)
                        return getColorByPrice(m_pSnapTable[row].PreClosePrice, m_pSnapTable[row].LastPrice );
                    else if(col == 5)
                        return getColorByPrice(m_pSnapTable[row].PreClosePrice,m_pSnapTable[row].BuyItem[0].Price);
                    else if(col == 7)
                        return getColorByPrice(m_pSnapTable[row].PreClosePrice,m_pSnapTable[row].SellItem[0].Price);
                }

            }
        }
        else if (role == Qt::DisplayRole)
        {
            int row = index.row();
            int col = index.column();

            if(row <0 || row >= m_nDataSize)
            {
                return  QVariant();
            }

            if(col <0 || col >= 11)
            {
                return QVariant();
            }

            QString str = "";

            if(m_pSnapTable[row].ExCode.ExchID ==0)
            {
                if(col == 1)
                {
                    str = m_pCodeTable[row].Code;
                    return str;
                }
                else
                {
                    return QVariant();
                }
            }

            switch(col)
            {
            case 0:
            {
                int msec = m_pSnapTable[row].UpdateTime%1000;
                int ntime = m_pSnapTable[row].UpdateTime/1000;
                str.sprintf("%02d:%02d:%02d.%03d" , ntime/10000, (ntime%10000)/100, ntime%100 , msec);
            }
                break;
            case 1:str = m_pSnapTable[row].ExCode.Code; break;
            case 2:str.sprintf("%.04lf", m_pSnapTable[row].LastPrice);break;
            case 3:str.sprintf("%d",m_pSnapTable[row].Volume);break;
            case 4:str.sprintf("%d", m_pSnapTable[row].OpenInterest);break;
            case 5:str.sprintf("%.04lf", m_pSnapTable[row].BuyItem[0].Price);break;
            case 6:str.sprintf("%d", m_pSnapTable[row].BuyItem[0].Volume);break;
            case 7:str.sprintf("%.04lf", m_pSnapTable[row].SellItem[0].Price);break;
            case 8:str.sprintf("%d", m_pSnapTable[row].SellItem[0].Volume);break;
            case 9:str.sprintf("%.04lf",m_pSnapTable[row].UpperLimitPrice);break;
            case 10:str.sprintf("%.04lf",m_pSnapTable[row].LowerLimitPrice);break;
            }

            return str;
        }
        return QVariant();
    }

    QVariant    headerData(int section, Qt::Orientation orientation,  int role) const
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
            case 0:str = "Time";break;
            case 1:str = "Code";break;
            case 2:str = "LastPrice"; break;
            case 3:str = "Volume"; break;
            case 4:str = "OpenInterest"; break;
            case 5:str = "Buy1Price"; break;
            case 6:str = "Buy1Vol"; break;
            case 7:str = "Sell1Price"; break;
            case 8:str = "Sell1Vol"; break;
            case 9:str = "UpperPrice"; break;
            case 10:str = "LowerPrice"; break;
            }
            return str;
        }
    }

private:
    tagXTInstrument*                m_pCodeTable;
    tagXTSnapshot*                  m_pSnapTable;
    int                             m_nDataSize;
    bool                            m_bBlank;
};







//============================================================================================================================================================================================================================
DebugQuote::DebugQuote(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::debugquote)
{
    ui->setupUi(this);

    _curExchange = nullptr;

    setAttribute(Qt::WA_DeleteOnClose);

    initControls();

    _timer = new QTimer(this);
    QObject::connect(_timer, SIGNAL(timeout()), this, SLOT(ontimer()));
    _timer->start(500);

}

DebugQuote::~DebugQuote()
{
    if(ui)
    {
        delete ui;
        ui = nullptr;
    }
    if(_timer)
    {
        delete _timer;
        _timer = nullptr;
    }
}

void    DebugQuote::initControls()
{
    _exchIco.addFile(":/image/35.ico");
    _productIco.addFile(":/image/306.bmp");

    //[1]combox
    {
        ui->comboBox->setEditable(false);
        ui->comboBox->setView(new QListView() );

        std::vector<QExchange*> arr;
        GetDataModule()->GetExchangeList(arr);

        int ncount = arr.size();
        for(int i=0; i<ncount; i++)
        {
            QExchange* p = arr[i];
            char szbuf[255]={0};
            p->GetExchName(szbuf);
            QString strName = szbuf;
            QVariant v = QVariant::fromValue((void *) p);
            ui->comboBox->addItem(_exchIco, strName,  v );
        }
    }

    //[2]tableview
    {
        _customModel = new QuoteDataModel();
        ui->tableView->setModel(_customModel);

        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    }

    //[3]
    QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combox_indexChange(int)) );
    QObject::connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(on_listwidget_selectChange()));

    ui->comboBox->setCurrentIndex(0);
    on_combox_indexChange(0);

}


void    DebugQuote::on_combox_indexChange(int )
{
    //[1]listWidget
    ui->listWidget->clear();

    //int curindex = ui->comboBox->currentIndex();
    QVariant v = ui->comboBox->currentData();
    QExchange* p = (QExchange *) v.value<void *>();

    _curExchange = p;

    int ncount = p->GetProductList(nullptr, 0);
    if(ncount >0)
    {
        tagXTProduct* f = new tagXTProduct[ncount];
        p->GetProductList(f, ncount);

        for(int i=0; i<ncount; i++)
        {
            QString strID = f[i].ProductID;
            std::string str = f[i].Name;
            QString strName = XTCodec::AfGbk_ToQString(str);
            QString strVal = strID + "\t[" + strName + "]";

            QListWidgetItem *pItem = new QListWidgetItem(ui->listWidget);
            pItem->setData(Qt::UserRole, f[i].InnerProductID );  // 用户数据
            pItem->setText(strVal);
            pItem->setIcon(_productIco);

            ui->listWidget->addItem( pItem );
        }

        delete []f;
    }

    //[2]
    ui->listWidget->setCurrentRow(0);
}

void    DebugQuote::on_listwidget_selectChange()
{
    QListWidgetItem* pcur = ui->listWidget->currentItem();
    if(nullptr == pcur)
    {
        _customModel->setQuoteData(0, 0);
        return;
    }

    int nID = pcur->data(Qt::UserRole).toInt();  // 获取用户数据
    if(nullptr == _curExchange)
    {
        _customModel->setQuoteData(0, 0);
        return;
    }

    _customModel->setQuoteData(_curExchange, nID);

    //
}

void DebugQuote::closeEvent(QCloseEvent *event)
{
    _curExchange = nullptr;
    if(_customModel)
    {
        _customModel->release();
        delete _customModel;
        _customModel = nullptr;
    }

    event->accept();
}

void    DebugQuote::ontimer()
{
    if(_customModel)
    {
        _customModel->doreset();
    }
}
