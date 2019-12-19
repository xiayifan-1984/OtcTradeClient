#include "positionTable.h"
#include "datamodule.h"

//===========================================================================================================================
CLogTableView::CLogTableView(QWidget* parent):
    QTableView(parent)
{
    _stdModel = nullptr;

    initControls();
}

CLogTableView::~CLogTableView()
{
    if (_stdModel)
    {
        delete _stdModel;
        _stdModel = nullptr;
    }
    
}

void    CLogTableView::initControls()
{
    _stdModel = new QStandardItemModel();
    this->setModel(_stdModel);

    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    this->verticalHeader()->hide();
    this->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

    initHeaders();
}

void    CLogTableView::initHeaders()
{
    char* parrstr[4] =  {"账户", "账户名称", "时间", "详细信息"};
    int   arrwidth[4]= { 100, 100, 160, 400};

    for (size_t i = 0; i < 4; i++)
    {
        QStandardItem* pItem = new QStandardItem();
        pItem->setText( parrstr[i] );
        _stdModel->setHorizontalHeaderItem(i, pItem);
        this->setColumnWidth(i, arrwidth[i]);
    }
    
}

//===========================================================================================================================
class CContractDataModel : public QAbstractTableModel
{
public:
    CContractDataModel(QObject *parent = nullptr)
    {
        m_pNameTable = nullptr;
        m_nDataSize =0;
    }

    void        release()
    {
        if(m_pNameTable)
        {
            delete []m_pNameTable;
            m_pNameTable =nullptr;
        }
        m_nDataSize =0;
    }

    void        setStaticData()
    {
        //刷新所有视图数据
        beginResetModel();

        release();

        std::vector<tagXTNameTable>  arrName;
        GetDataModule()->GetAllNameTableList(arrName);

        int ncount = arrName.size();

        if(ncount >0)
        {
            m_pNameTable = new tagXTNameTable[ncount];
            for (size_t i = 0; i < ncount; i++)
            {
                m_pNameTable[i] = arrName[i];
            }
        }
        m_nDataSize = ncount;
        

        endResetModel();
    }

    int         rowCount(const QModelIndex &parent) const
    {
        return m_nDataSize;
    }
    int         columnCount(const QModelIndex &parent) const
    {
        return 8;
    }
    QVariant    data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        if (role == Qt::TextAlignmentRole)
        {
            return int(Qt::AlignRight | Qt::AlignVCenter);
        }
        else if (role == Qt::DisplayRole)
        {
            int row = index.row();
            int col = index.column();

            if(row <0 || row >= m_nDataSize)
            {
                return  QVariant();
            }

            if(col <0 || col >= 8)
            {
                return QVariant();
            }

            QString str = "";

            switch(col)
            {
            case 0:str = m_pNameTable[row].ExCode.Code;break;
            case 1:str = m_pNameTable[row].ProductID;break;
            case 2:str.sprintf("%d",  m_pNameTable[row].ProductClass );break;
            case 3:str.sprintf("%d", m_pNameTable[row].Delivery); break;
            case 4:str.sprintf("%d", m_pNameTable[row].VolumeMultiple); break;
            case 5:str.sprintf("%.04lf", m_pNameTable[row].PriceTick); break;
            case 6:str.sprintf("%d", m_pNameTable[row].OpenDate);break;
            case 7:str.sprintf("%d", m_pNameTable[row].ExpireDate);break;
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
            case 0:str = "合约";break;
            case 1:str = "品种";break;
            case 2:str = "品种类别"; break;
            case 3:str = "交割月"; break;
            case 4:str = "合约乘数"; break;
            case 5:str = "PriceTick"; break;
            case 6:str = "上市日"; break;
            case 7:str = "到期日"; break;
            }
            return str;
        }
    }

private:
    tagXTNameTable*     m_pNameTable;
    int                 m_nDataSize;
};

CContractTableView::CContractTableView(QWidget* parent):
    QTableView(parent)
{
    _dataModel = nullptr;

    initControls();
}

CContractTableView::~CContractTableView()
{
    if (_dataModel)
    {
        _dataModel->release();
        delete _dataModel;
        _dataModel = nullptr;
    }
    
}

void    CContractTableView::initControls()
{
    _dataModel = new CContractDataModel();
    this->setModel(_dataModel);

    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    this->verticalHeader()->hide();
    this->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

    _dataModel->setStaticData();
}









