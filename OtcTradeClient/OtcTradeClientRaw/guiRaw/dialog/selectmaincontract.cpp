
#include "selectmaincontract.h"
#include "datamodule.h"
#include "XTCodec.h"
#include "configmodule.h"

SelectMainContract::SelectMainContract(QWidget* parent/* = nullptr*/)
{
    setWindowTitle(tr("Select Main Contract"));

    initControls();

    initListCode();
}

SelectMainContract::~SelectMainContract()
{

}

void        SelectMainContract::initControls()
{
    //[1]
    {
        _lblTip = new QLabel();
        _lblTip->setText(tr("Select one:"));

        _listCode = new QListWidget();

        _btnOK = new QPushButton(tr("OK"));
        _btnCancel = new QPushButton(tr("Cancel"));
    }

    //[2]
    {
        QHBoxLayout* h2 = new QHBoxLayout();
        h2->addStretch();
        h2->addWidget(_btnOK);
        h2->addWidget(_btnCancel);

        QVBoxLayout* mainlayout = new QVBoxLayout();
        mainlayout->addWidget(_lblTip);
        mainlayout->addWidget(_listCode);
        mainlayout->addLayout(h2);

        setLayout(mainlayout);
    }

    //[3]
    QObject::connect(_btnOK,SIGNAL(clicked()), this, SLOT(onOK() ) );
    QObject::connect(_btnCancel, SIGNAL(clicked()), this, SLOT(onCancel() ));
    QObject::connect(_listCode, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(onlistcodeDoubleCliecked(QListWidgetItem*)) );

}

void     SelectMainContract::initListCode()
{
    vector<tagXTInstrument> arrExCode;
    int ncount = GetConfigModule()->GetAllMainContract(arrExCode);
    for(int i=0; i<ncount; i++)
    {
        tagXTInstrument oExCode = arrExCode[i];
        QExchange* _curExchange = GetDataModule()->GetExchange(oExCode.ExchID);
        if(nullptr == _curExchange)
        {
            continue;
        }
        if(!_curExchange->IsExistCode(oExCode))
        {
            continue;
        }

        QString strCode = oExCode.Code;
        QString strName;
        {

            tagXTNameTable oNameTable;
            _curExchange->GetOneNameTable(&oExCode, &oNameTable);

            std::string str = oNameTable.Name;
            strName = XTCodec::AfGbk_ToQString(str);
        }

        QString strVal = strCode + "\t[" + strName + "]";

        QListWidgetItem *pItem = new QListWidgetItem(_listCode);

        QVariant v3 = QVariant::fromValue(oExCode);
        pItem->setData(Qt::UserRole, v3); // 用户数据

        pItem->setText(strVal);

        _listCode->addItem( pItem );
    }
}

void     SelectMainContract::getSelectCommodity(tagXTInstrument* pIn)
{
    *pIn = _curselCode;
}

void    SelectMainContract::onOK()
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

void    SelectMainContract::onCancel()
{
    reject();
}


void  SelectMainContract::onlistcodeDoubleCliecked(QListWidgetItem* p)
{
    if(p)
    {
        onOK();
    }
}








