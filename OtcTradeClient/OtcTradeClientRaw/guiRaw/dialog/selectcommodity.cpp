
#include "selectcommodity.h"
#include "datamodule.h"
#include "XTCodec.h"
#include "updownlineedit.h"

SelectCommodity::SelectCommodity(QWidget* parent/* = nullptr*/)
{
    _curExchange = nullptr;

    setWindowTitle(tr("Select Commodity"));

    initControls();

}

SelectCommodity::~SelectCommodity()
{

}

void        SelectCommodity::initControls()
{
    //[1]
    {
        _lblContract = new QLabel();
        _lblContract->setText(tr("Input:"));

        _lblExchange = new QLabel();
        _lblExchange->setText(tr("Exchange:"));

        _editContract = new UpDownLineEdit();

        _cboxExchange = new QComboBox();

        _listProduct = new QListWidget();
        _listProduct->setMaximumWidth(200);

        _listCode = new QListWidget();

        _btnOK = new QPushButton(tr("OK"));
        _btnCancel = new QPushButton(tr("Cancel"));
    }

    //[2]
    {
        QGridLayout* g1 = new QGridLayout();
        g1->addWidget(_lblContract, 0, 0);
        g1->addWidget(_lblExchange, 1, 0);
        g1->addWidget(_editContract, 0, 1);
        g1->addWidget(_cboxExchange, 1, 1);

        QVBoxLayout* v1 = new QVBoxLayout();
        v1->addLayout(g1);
        v1->addWidget(_listProduct);

        QHBoxLayout* h1 = new QHBoxLayout();
        h1->addLayout(v1);
        h1->addWidget(_listCode, 2);

        QHBoxLayout* h2 = new QHBoxLayout();
        h2->addStretch();
        h2->addWidget(_btnOK);
        h2->addWidget(_btnCancel);

        QVBoxLayout* mainlayout = new QVBoxLayout();
        mainlayout->addLayout(h1);
        mainlayout->addLayout(h2);

        setLayout(mainlayout);
    }

    //[3]
    QObject::connect(_btnOK,SIGNAL(clicked()), this, SLOT(onOK() ) );
    QObject::connect(_btnCancel, SIGNAL(clicked()), this, SLOT(onCancel() ));
    QObject::connect(_editContract, SIGNAL(textChanged(const QString&)), this, SLOT(onCodetextChanged(const QString&))  );
    QObject::connect(_listCode, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(onlistcodeDoubleCliecked(QListWidgetItem*)) );

    QObject::connect(_editContract, SIGNAL(editupkey()), this, SLOT(oneditkeyup()) );
    QObject::connect(_editContract, SIGNAL(editdownkey()), this, SLOT(oneditkeydown()) );

    //[4]combox
    {
        {
            _cboxExchange->setEditable(false);
            _cboxExchange->setView(new QListView() );

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
                _cboxExchange->addItem(strName,  v );
            }
        }
    }

    //[5]
    QObject::connect(_cboxExchange, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combox_indexChange(int)) );
    QObject::connect(_listProduct, SIGNAL(itemSelectionChanged()), this, SLOT(on_listwidget_selectChange()));

    _cboxExchange->setCurrentIndex(0);
    on_combox_indexChange(0);
}


void    SelectCommodity::on_combox_indexChange(int )
{
    //[1]listWidget
    _listProduct->clear();

    QVariant v = _cboxExchange->currentData();
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

            QListWidgetItem *pItem = new QListWidgetItem(_listProduct);
            pItem->setData(Qt::UserRole, f[i].InnerProductID );  // 用户数据
            pItem->setText(strVal);

            _listProduct->addItem( pItem );
        }

        delete []f;
    }

    //[2]
    _listProduct->setCurrentRow(0);
}


void    SelectCommodity::on_listwidget_selectChange()
{
    _listCode->clear();

    QListWidgetItem* pcur = _listProduct->currentItem();
    if(nullptr == pcur)
    {
        return;
    }

    int nID = pcur->data(Qt::UserRole).toInt();  // 获取用户数据
    if(nullptr == _curExchange)
    {
        return;
    }

    int ncount = _curExchange->GetCodeListByProduct(nullptr, 0, nID);
    if(ncount >0)
    {
        tagXTInstrument* pcode = new tagXTInstrument[ncount];
        _curExchange->GetCodeListByProduct(pcode, ncount, nID);

        for(int i = 0; i < ncount; i++)
        {
            QString strCode = pcode[i].Code;
            char szname[64]={0};
            _curExchange->GetExchName(szname);
            QString strExch = QString("%1").arg(szname);
            QString strVal = strCode + "\t[" + strExch + "]";

            QListWidgetItem *pItem = new QListWidgetItem(_listCode);

            QVariant v3 = QVariant::fromValue(pcode[i]);
            pItem->setData(Qt::UserRole, v3); // 用户数据

            pItem->setText(strVal);

            _listCode->addItem( pItem );

        }

        delete []pcode;
    }

}

void     SelectCommodity::getSelectCommodity(tagXTInstrument* pIn)
{
    *pIn = _curselCode;
}

void    SelectCommodity::onOK()
{
    //检查
    if( _listCode->count() <= 0)
    {
        return;
    }

    QListWidgetItem* pcur = _listCode->currentItem();
    if(pcur == nullptr)
    {
        return;
    }

    QVariant v2 = pcur->data(Qt::UserRole);
    if (v2.canConvert<tagXTInstrument>())
    {
        tagXTInstrument card = v2.value<tagXTInstrument>();
        _curselCode = card;

        //选择一个合适的代码后，返回
        accept();
    }

}

void    SelectCommodity::onCancel()
{
    reject();
}

void    SelectCommodity::onCodetextChanged(const QString &text)
{
    //输入代码的Edit发生改变
    int nlen = text.length();
    if(nlen <=0)
    {
        _listCode->clear();
        return;
    }

    //[1]
    std::string str = XTCodec::AfUtf8_ToString(text);
    std::vector<tagXTInstrument> arrTarget;

    //[2]
    std::vector<QExchange*> arrExch;
    GetDataModule()->GetExchangeList(arrExch);
    for(int i=0; i< arrExch.size(); i++)
    {
        QExchange* p = arrExch[i];
        int nsize = p->GetCodeList(nullptr, 0);
        if(nsize <= 0)
        {
            continue;
        }
        tagXTInstrument* parrcode = new tagXTInstrument[nsize];
        p->GetCodeList(parrcode, nsize);
        for(int j =0; j<nsize; j++)
        {
            char* pcode = parrcode[j].Code;
            if(strnicmp(pcode, str.c_str(), nlen) == 0)
            {
                arrTarget.push_back(parrcode[j]);
            }
        }
        delete []parrcode;
    }

    _listCode->clear();
    for(int k = 0; k < arrTarget.size(); k++)
    {
        QString strCode = arrTarget[k].Code;

        QString strVal;
        QExchange* pexch = GetDataModule()->GetExchange(arrTarget[k].ExchID);
        if(pexch)
        {
            char szname[64]={0};
            pexch->GetExchName(szname);
            QString strExch = QString("%1").arg(szname);
            strVal = strCode + "\t[" + strExch + "]";
        }
        else
        {
            strVal = strCode;
        }

        QListWidgetItem *pItem = new QListWidgetItem(_listCode);

        QVariant v3 = QVariant::fromValue(arrTarget[k]);
        pItem->setData(Qt::UserRole, v3); // 用户数据

        pItem->setText(strVal);

        _listCode->addItem( pItem );

    }

    if( arrTarget.size() >0 )
    {
        _listCode->setCurrentRow(0);
    }

}

void  SelectCommodity::onlistcodeDoubleCliecked(QListWidgetItem* p)
{
    if(p)
    {
        onOK();
    }
}


void        SelectCommodity::oneditkeyup()
{
    int nsize = _listCode->count();
    if(nsize <=0)
    {
        return;
    }

    int curRow = _listCode->currentRow();
    if(curRow <=0)
    {
        return;
    }

    curRow--;
    _listCode->setCurrentRow(curRow);
}

void        SelectCommodity::oneditkeydown()
{
    int nsize = _listCode->count();
    if(nsize <=0)
    {
        return;
    }

    int curRow = _listCode->currentRow();
    if(curRow >= nsize-1)
    {
        return;
    }

    curRow++;
    _listCode->setCurrentRow(curRow);
}





