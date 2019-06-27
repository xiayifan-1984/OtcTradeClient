#include "datastaticdata.h"
#include "datamodule.h"
#include "XTCodec.h"

//============================================================================================================================================================================================================================
class StaticDataModel : public QAbstractTableModel
{
public:
    StaticDataModel(QObject *parent = nullptr)
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

    void        setStaticData(QExchange* p, int productID)
    {
        //刷新所有视图数据
        beginResetModel();

        release();

        if(p)
        {
            int ncount = p->GetCodeListByProduct(nullptr, 0, productID);

            if(ncount >0)
            {
                m_pNameTable = new tagXTNameTable[ncount];
                p->GetNameListByProduct(m_pNameTable, ncount, productID);
            }
            m_nDataSize = ncount;
        }

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
            case 0:str = "Code";break;
            case 1:str = "Product";break;
            case 2:str = "ProductClass"; break;
            case 3:str = "Delivery"; break;
            case 4:str = "Multiple"; break;
            case 5:str = "PriceTick"; break;
            case 6:str = "StartDate"; break;
            case 7:str = "EndDate"; break;
            }
            return str;
        }
    }

private:
    tagXTNameTable*     m_pNameTable;
    int                 m_nDataSize;
};

//============================================================================================================================================================================================================================

DataStaticData::DataStaticData()
{
    initUI();

    _curExchange = nullptr;

    setAttribute(Qt::WA_DeleteOnClose);

    initControls();
}

DataStaticData::~DataStaticData()
{

}

void       DataStaticData::initUI()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("DataStaticData"));

    this->resize(600, 500);

    horizontalLayout = new QHBoxLayout();
    {
        verticalLayout = new QVBoxLayout();

        comboBox = new QComboBox();
        verticalLayout->addWidget(comboBox);

        treeWidget = new QTreeWidget();
        verticalLayout->addWidget(treeWidget);

        codetblWidget = new QTableView();

        horizontalLayout->addLayout(verticalLayout);
        horizontalLayout->addWidget(codetblWidget);
    }

    horizontalLayout->setStretch(0, 2);
    horizontalLayout->setStretch(1, 3);
    this->setLayout(horizontalLayout);

    this->setWindowTitle(tr("DataNameTable"));
}

void       DataStaticData::initControls()
{
    _exchIco.addFile(":/image/35.ico");
    _productIco.addFile(":/image/306.bmp");

    //[1]combox
    {
        comboBox->setEditable(false);
        comboBox->setView(new QListView() );

        std::vector<QExchange*> arr;
        GetDataModule()->GetExchangeList(arr);

        int ncount = arr.size();
        for(int i=0; i<ncount; i++)
        {
            QExchange* p = arr[i];
            char szName[255]={0};
            p->GetExchName(szName);

            QString strName = szName;
            QVariant v = QVariant::fromValue((void *) p);
            comboBox->addItem(_exchIco, strName,  v );
        }
    }

    //[2]codetblWidget
    {
            _customModel = new StaticDataModel();
            codetblWidget->setModel(_customModel);

            codetblWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
            codetblWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    }

    //[3]treewidget
    {
        treeWidget->setColumnCount(1);
        treeWidget->setHeaderHidden(true);     //隐藏Header
        treeWidget->setItemsExpandable(false);
        //隐藏root的折叠图标
        treeWidget->setRootIsDecorated(false);
    }

    //[4]
    QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combox_indexChange(int)) );
    QObject::connect(treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(on_treewidget_selectChange()));

    comboBox->setCurrentIndex(0);
    on_combox_indexChange(0);
}

void       DataStaticData::on_combox_indexChange(int index)
{
    //[1]listWidget
    treeWidget->clear();

    //int curindex = comboBox->currentIndex();
    QVariant v = comboBox->currentData();
    QExchange* p = (QExchange *) v.value<void *>();

    _curExchange = p;

    QTreeWidgetItem* _root1 = new QTreeWidgetItem(treeWidget, QStringList(QString("Future")), itemRootFuture);
    QTreeWidgetItem* _root2 = new QTreeWidgetItem(treeWidget, QStringList(QString("Option")), itemRootOption);

    //[2]
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

            if(f[i].ProductClass == 1)  //期货
            {
                QTreeWidgetItem* pItem = new QTreeWidgetItem(itemFuture);
                pItem->setText(0, strVal);
                pItem->setData(0, Qt::UserRole, f[i].InnerProductID);
                pItem->setIcon(0, _productIco);
                _root1->addChild(pItem);
            }
            else if(f[i].ProductClass == 2) //期权
            {
                QTreeWidgetItem* pItem = new QTreeWidgetItem(itemOption);
                pItem->setText(0, strVal);
                pItem->setData(0, Qt::UserRole, f[i].InnerProductID);
                pItem->setIcon(0, _productIco);
                _root2->addChild(pItem);
            }

        }

        delete []f;
    }

    //[2]
    //ui->listWidget->setCurrentRow(0);
    if(_root1->childCount() >0)
    {
        treeWidget->setCurrentItem( _root1->child(0) );
    }

    //[3]
    {
        _root1->setTextColor(0, QColor(0,0,0));
        _root1->setBackgroundColor(0, QColor(225,225,225));
        QIcon icon;
        icon.addFile(":/image/516.bmp");
        _root1->setIcon(0, icon);
    }
    {
        _root2->setTextColor(0, QColor(0,0,0));
        _root2->setBackgroundColor(0, QColor(225,225,225));
        QIcon icon;
        icon.addFile(":/image/230.bmp");
        _root2->setIcon(0, icon);
    }

    treeWidget->expandAll();
}

void       DataStaticData::on_treewidget_selectChange()
{
    QTreeWidgetItem* pcur = treeWidget->currentItem();
    if(nullptr == pcur)
    {
        _customModel->setStaticData(0, 0);
        return;
    }
    if(itemFuture != pcur->type() && itemOption != pcur->type() )
    {
        _customModel->setStaticData(0, 0);
        return;
    }

    if(nullptr == _curExchange)
    {
        _customModel->setStaticData(0, 0);
        return;
    }

    int nID = pcur->data(0, Qt::UserRole).toInt();  // 获取用户数据

    _customModel->setStaticData(_curExchange, nID);

}

void DataStaticData::closeEvent(QCloseEvent *event)
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




