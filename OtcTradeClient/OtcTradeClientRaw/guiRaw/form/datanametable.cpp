#include "datanametable.h"
#include "datamodule.h"
#include "XTCodec.h"
#include <set>
using namespace std;

DataNameTable::DataNameTable()
{
    initUI();

    _curExchange = nullptr;

    setAttribute(Qt::WA_DeleteOnClose);

    initControls();
}

DataNameTable::~DataNameTable()
{

}

void       DataNameTable::initUI()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("DataNameTable"));

    this->resize(600, 500);

    horizontalLayout = new QHBoxLayout();
    {
        verticalLayout = new QVBoxLayout();

        comboBox = new QComboBox();
        verticalLayout->addWidget(comboBox);

        treeWidget = new QTreeWidget();
        verticalLayout->addWidget(treeWidget);

        tabWidget = new QTabWidget();
        {
            codetblWidget = new QTableWidget();
            tabWidget->addTab(codetblWidget, QIcon(":/image/412.bmp"), "List");

            opttblWidget = new QTableWidget();
            tabWidget->addTab(opttblWidget, QIcon(":/image/410.bmp"), "Grid");
        }

        horizontalLayout->addLayout(verticalLayout);
        horizontalLayout->addWidget(tabWidget);
    }

    horizontalLayout->setStretch(0, 2);
    horizontalLayout->setStretch(1, 3);
    this->setLayout(horizontalLayout);

    this->setWindowTitle(tr("DataNameTable"));
}

void       DataNameTable::initControls()
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
        codetblWidget->setColumnCount(2);
        QStringList header;
        header<<"Exchange"<<"Code";
        codetblWidget->setHorizontalHeaderLabels(header);

        codetblWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        codetblWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
        codetblWidget->horizontalHeader()->setStretchLastSection(true);
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

void       DataNameTable::on_combox_indexChange(int index)
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

void       DataNameTable::on_treewidget_selectChange()
{
    resetCodeList();
    resetCodeGrid();

    QTreeWidgetItem* pcur = treeWidget->currentItem();
    if(nullptr == pcur)
    {
        return;
    }
    if(itemFuture != pcur->type() && itemOption != pcur->type() )
    {
        return;
    }

    if(nullptr == _curExchange)
    {
        return;
    }

    int nID = pcur->data(0, Qt::UserRole).toInt();  // 获取用户数据

    initCodeList(nID);

    if(pcur->type() == itemOption)
    {
        initCodeGrid(nID);
    }
}

void    DataNameTable::resetCodeList()
{
    codetblWidget->clearContents();
    codetblWidget->setRowCount(0);
}

void   DataNameTable::initCodeList(int nID)
{
    int ncount = _curExchange->GetCodeListByProduct(nullptr, 0, nID);
    codetblWidget->setRowCount(ncount);

    if(ncount >0)
    {
        tagXTInstrument* pcode = new tagXTInstrument[ncount];
        _curExchange->GetCodeListByProduct(pcode, ncount, nID);

        for(int i = 0; i < ncount; i++)
        {
            int temp = pcode[i].ExchID;
            QString strExch;
            strExch.sprintf("0x%02X", temp); //QString("%1").arg(temp);
            QString strCode = pcode[i].Code;
            codetblWidget->setItem(i, 0, new QTableWidgetItem(strExch));
            codetblWidget->setItem(i, 1, new QTableWidgetItem(strCode));
        }

        delete []pcode;
    }
}

void   DataNameTable::resetCodeGrid()
{
    opttblWidget->clear();
    opttblWidget->setColumnCount(0);
    opttblWidget->setRowCount(0);
}

void   DataNameTable::initCodeGrid(int nID)
{
    int ncount = _curExchange->GetNameListByProduct(nullptr, 0, nID);
    if(ncount <=0)
    {
        return;
    }

    tagXTNameTable* ptable = new tagXTNameTable[ncount];
    _curExchange->GetNameListByProduct(ptable, ncount, nID);

    set<int>  allPrice;
    set<int>  allDate;

    for(int i = 0; i < ncount; i++)
    {
        allPrice.insert(ptable[i].ExecPrice);
        allDate.insert(ptable[i].ExpireDate);
    }

    int rowcount = allPrice.size();
    int colcount = allDate.size();
    int* parrGrid = new int[rowcount*colcount];
    memset(parrGrid, 0, sizeof(int)*rowcount*colcount );
    for(int i=0; i< ncount; i++)
    {
        //[get row]
        int row =0;
        set<int>::iterator itb = allPrice.begin();
        while(itb != allPrice.end() )
        {
            if(*itb == ptable[i].ExecPrice)
            {
                break;
            }
            itb++;
            row++;
        }

        if(itb == allPrice.end())
        {
            continue;
        }

        int col =0;
        itb= allDate.begin();
        while(itb != allDate.end() )
        {
            if(*itb == ptable[i].ExpireDate)
            {
                break;
            }
            itb++;
            col++;
        }

        if(itb == allDate.end() )
        {
            continue;
        }

        parrGrid[row * colcount + col] = 1;
    }

    delete []ptable;

    //[2]init
    {
        opttblWidget->setColumnCount(colcount);
        set<int>::iterator itb = allDate.begin();
        int col =0;
        while(itb != allDate.end() )
        {
            QString str;
            str.sprintf("%d", *itb);
            QTableWidgetItem* p = new QTableWidgetItem( str );
            opttblWidget->setHorizontalHeaderItem(col , p);
            opttblWidget->setColumnWidth(col, 50);

            itb++;
            col++;
        }

        opttblWidget->setRowCount(rowcount);
        int row =0;
        itb = allPrice.begin();
        while(itb != allPrice.end() )
        {
            QString str;
            str.sprintf("%d", (*itb)/10000 );
            QTableWidgetItem* p = new QTableWidgetItem( str);
            opttblWidget->setVerticalHeaderItem(row, p);
            itb++;
            row++;
        }
        opttblWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //不允许编辑
        opttblWidget->setSelectionBehavior(QAbstractItemView::SelectItems); //整行选中的方式
        //opttblWidget->horizontalHeader()->setStretchLastSection(true);

    }

    //[3]
    for(int i=0; i<rowcount; i++)
    {
        for(int j=0; j<colcount; j++)
        {
            if(parrGrid[i*colcount + j] == 1)
            {
                QTableWidgetItem* p = new QTableWidgetItem();
                p->setCheckState(Qt::Unchecked );
                p->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//文本对齐格式
                opttblWidget->setItem(i, j, p );
            }
        }
    }

    delete []parrGrid;
}




