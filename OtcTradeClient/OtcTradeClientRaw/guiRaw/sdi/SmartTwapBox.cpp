
#include "SmartTwapBox.h"
#include "datamodule.h"
#include "XTCodec.h"
#include "configmodule.h"
#include "trademodule.h"
#include "stool.h"
#include "codeinnermsg.h"
#include "twapordermgr.h"

CSmartTwapBox::CSmartTwapBox(QWidget* parent /*= nullptr*/)
{
    setAttribute(Qt::WA_DeleteOnClose);
    memset(&m_oExCode, 0, sizeof(tagXTInstrument));


    initControls();

    configControls();
}

CSmartTwapBox::~CSmartTwapBox()
{

}

void       CSmartTwapBox::initControls()
{
    //一个GridLayout
    QGridLayout* g1 = new QGridLayout();

    //第一行
    {
        _lblCode = new QLabel(tr("合约:"));
        _editCode = new QLineEdit();

        //布局
        g1->addWidget(_lblCode, 0, 0);
        g1->addWidget(_editCode, 0, 1);

        initCompleter();
    }

    //第二行
    {
        _lblAccount = new QLabel(tr("账号:"));
        _cboxAccount = new QComboBox();

        g1->addWidget(_lblAccount, 1, 0);
        g1->addWidget(_cboxAccount, 1,1);
    }

    //第三行
    {
        _lblDirection = new QLabel(tr("买卖"));
        _rbtnBuy = new QRadioButton(tr("买入"));
        _rbtnSell = new QRadioButton(tr("卖出"));

        //把几个Radio编成组，在组内互斥
        _gbtnDirection = new QButtonGroup();
        _gbtnDirection->addButton(_rbtnBuy, 0);
        _gbtnDirection->addButton(_rbtnSell, 1);
        QObject::connect( _gbtnDirection, SIGNAL(buttonToggled(int, bool)), this, SLOT(on_bgGroup_direction_toggled(int, bool)));


        //布局
        QHBoxLayout* h2 = new QHBoxLayout();
        h2->addWidget(_rbtnBuy);
        h2->addWidget(_rbtnSell);
        h2->addStretch();

        g1->addWidget(_lblDirection, 2, 0);
        g1->addLayout(h2, 2, 1);
    }

    //第四行
    {
        _lblOffset = new QLabel(tr("开平:"));
        _rbtnOpen = new QRadioButton(tr("开仓"));
        _rbtnClose = new QRadioButton(tr("平仓"));
        _rbtnCloseToday  = new QRadioButton(tr("平今"));
        _rbtnAuto = new QRadioButton(tr("自动"));

        //把几个Radio编成组，在组内互斥
        _gbtnOffset = new QButtonGroup();
        _gbtnOffset->addButton(_rbtnOpen, 0);
        _gbtnOffset->addButton(_rbtnClose, 1);
        _gbtnOffset->addButton(_rbtnCloseToday, 2);
        _gbtnOffset->addButton(_rbtnAuto, 3);
        QObject::connect( _gbtnOffset, SIGNAL(buttonToggled(int, bool)), this, SLOT(on_bgGroup_offset_toggled(int, bool)));

        //布局
        QHBoxLayout* h3 = new QHBoxLayout();
        h3->addWidget(_rbtnOpen);
        h3->addWidget(_rbtnClose);
        h3->addWidget(_rbtnCloseToday);
        h3->addWidget(_rbtnAuto);
        h3->addStretch();

        g1->addWidget(_lblOffset, 3, 0);
        g1->addLayout(h3, 3, 1);
    }

    //第五行
    {
        _lblVol = new QLabel(tr("手数:"));
        _spinVol = new QSpinBox();
        g1->addWidget(_lblVol, 4, 0);
        g1->addWidget(_spinVol, 4, 1);
    }

    //第六行
    {
        _lblStart = new QLabel(tr("起始时间:"));
        _lblEnd = new QLabel(tr("终止时间:"));
        _twapBegin = new QTimeEdit();
        _twapBegin->setDisplayFormat("HH:mm:ss");
        _twapBegin->setTime(QTime::currentTime());
        _twapEnd = new QTimeEdit();
        _twapEnd->setDisplayFormat("HH:mm:ss");
        _twapEnd->setTime(QTime::fromString("14:59:59", "hh:mm:ss"));
        QGridLayout* g2 = new QGridLayout();
        g2->addWidget(_lblStart, 0, 0);
        g2->addWidget(_lblEnd, 0, 1);
        g2->addWidget(_twapBegin, 1, 0);
        g2->addWidget(_twapEnd, 1, 1);

        g1->addLayout(g2, 5, 0, 1, 2);
    }

    //第七行
    {
        _btnInput = new QPushButton(tr("下单"));

        g1->addWidget(_btnInput, 6, 1);
    }

    //总布局
    QHBoxLayout* sublayout = new QHBoxLayout();
    sublayout->addLayout(g1);
    sublayout->addStretch();

    QVBoxLayout* mainlayout = new QVBoxLayout();
    mainlayout->addLayout(sublayout);
    mainlayout->addStretch();

    this->setLayout(mainlayout);

    //[2]消息
    QObject::connect(_editCode, SIGNAL(returnPressed()), this, SLOT(onCodePressReturn()) );

    //[3]代码框，价格，数量框，设置为粗体
    const QFont oldfont = this->font();
    QFont newfont(oldfont);
    newfont.setBold(true);
    _editCode->setFont(newfont);
    _spinVol->setFont(newfont);

    //[4]数量 靠右侧显示
    _spinVol->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    //[5]消息
    QObject::connect(_btnInput, SIGNAL(clicked()), this,  SLOT(on_inputbtn_clicked()) );


}

void    CSmartTwapBox::configControls()
{
    //[0]默认选择： 默认为非套保
    _editCode->setPlaceholderText(tr("IF1912"));

    //[1]默认值
    _rbtnBuy->setChecked(true);
    _rbtnOpen->setChecked(true);

    //[2]spinVol默认值
    _spinVol->setMinimum(1);
    _spinVol->setMaximum(1000);
    _spinVol->setSingleStep(1);

    //[4]外观
    _rbtnBuy->setProperty("name", "buyradio");
    _rbtnOpen->setProperty("name", "openradio");
    _btnInput->setProperty("name", "buy");
    this->setProperty("name","smartinputbox");
}


void     CSmartTwapBox::initCompleter()
{
    _completer = new QCompleter();
    _codelistModel = new QStringListModel();

    _completer->setCaseSensitivity(Qt::CaseInsensitive);

    _completer->setModel(_codelistModel);
    _editCode->setCompleter(_completer);

    //[1]填充
    QStringList  arrCode;
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
            arrCode << pcode;
        }
        delete []parrcode;
    }

    //[2]
    _codelistModel->setStringList(arrCode);

}

void    CSmartTwapBox::onCodePressReturn()    //当输入代码，回车时
{
    QString strcode = _editCode->text();

    int nlen = strcode.length();
    if(nlen <=0)
    {
        return;
    }

    //[1]
    std::string str = XTCodec::AfUtf8_ToString(strcode);
    tagXTInstrument oExCode;
    memset(&oExCode, 0, sizeof(oExCode));
    strncpy(oExCode.Code, str.c_str(), 30);

    int iret = GetDataModule()->VerifyExCode(oExCode);
    if(iret <=0)
    {
        qDebug() << "onCodePressReturn " << "verifycode fail";
        return;
    }

    _editCode->clear();
    setCurCommodity(oExCode);
}

void    CSmartTwapBox::on_inputbtn_clicked()      //点击了下单按钮
{
    if(m_oExCode.ExchID <=0)
    {
        QString str = "交易品种未设置";
        QMessageBox::critical(nullptr, "TwapInputError", str, QMessageBox::Yes);
        return;
    }
    QTime begin = _twapBegin->time() < QTime::currentTime() ? QTime::currentTime() : _twapBegin->time();
    QTime end = _twapEnd->time();
    string tmp(m_oExCode.Code);

    auto number_ptr = find_if(tmp.begin(), tmp.end(), [](char c){return c >='0' && c <= '9';});
    string inst(tmp.begin(), number_ptr);
    inst = stool::strToLower(inst);
    if(!stool::isMarketAlive(inst, begin, end))
    {
        QString str = "非开盘时间";
        QMessageBox::critical(nullptr, "TwapInputError", str, QMessageBox::Yes);
        return;
    }
    inputTwapOrder(begin, end);

}

void    CSmartTwapBox::on_bgGroup_direction_toggled(int, bool)
{
    inner_setBtnText();

    if(_gbtnDirection->checkedId() ==0)
    {
        _btnInput->setProperty("name", "buy");
        _btnInput->style()->polish(_btnInput);
        _btnInput->update();
        setStyle(QApplication::style());  //必须加上这一句，否则不生效
    }
    else
    {
        _btnInput->setProperty("name", "sell");
        _btnInput->style()->polish(_btnInput);
        _btnInput->update();
        setStyle(QApplication::style());  //必须加上这一句，否则不生效
    }
}

void    CSmartTwapBox::on_bgGroup_offset_toggled(int, bool)
{
    inner_setBtnText();
}

void    CSmartTwapBox::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void    CSmartTwapBox::inner_setBtnText()
{
    QString strText = "买";
    int nid = _gbtnDirection->checkedId();
    char direction = XT_D_Buy;
    if(nid ==1)
    {
        direction = XT_D_Sell;
        strText = "卖";
    }

    nid = _gbtnOffset->checkedId();
    char offset = XT_OF_Open;
    switch (nid)
    {
        case 0:
        {
            offset = XT_OF_Open;
            strText += "  开";
        }
        break;
    case 1:
        {
            offset = XT_OF_Close;
            strText += "  平";
        }
        break;
    case 2:
        {
            offset = XT_OF_CloseToday;
            strText += "  平今";
        }
        break;
    case 3:
        {
            offset = XT_OF_Auto;
            strText += "  ";
        }
        break;
    }

    _btnInput->setText(strText);
}


void      CSmartTwapBox::setCurCommodity(tagXTInstrument& oCode)
{
    m_oExCode = oCode;

    tagXTNameTable oNameCode;
    QExchange* pExchange = GetDataModule()->GetExchange(oCode.ExchID);
    if(pExchange)
    {
        pExchange->GetOneNameTable(&oCode, &oNameCode);
    }

    //[2]
    std::string strcode = oCode.Code;
    QString trcode = XTCodec::AfGbk_ToQString(strcode);
    _editCode->setText(trcode);

    //[5]数量初始化为1
    _spinVol->setValue(1);

    //[6]初始化账号ComboBox
    initAccount(oNameCode.ProductID);
}

void    CSmartTwapBox::initAccount(const char* pProductID)
{
    vector<tagTTradeUserParam>  arrUser;
    GetConfigModule()->GetTradeUserBy(pProductID, arrUser);
    qDebug() << "KBInputBox initAccount number " << arrUser.size();

    _cboxAccount->clear();
    int nsize = arrUser.size();
    for(int i = 0; i< nsize; i++)
    {
        tagTTradeUserParam& o = arrUser[i];
        QOrderMgr* p = GetTradeModule()->GetMgr(o.type, o.broker, o.user);

        QString strName;
        std::string strmsg = o.aliasuser;
        strName = XTCodec::AfGbk_ToQString(strmsg);

        QVariant v = QVariant::fromValue((void *)p);
        _cboxAccount->addItem(strName, v);
    }

}

int    CSmartTwapBox::inputTwapOrder(QTime& from, QTime& to)
{
    //[0]
    int nid = _gbtnDirection->checkedId();
    char direction = XT_D_Buy;
    if(nid ==1)
    {
        direction = XT_D_Sell;
    }

    nid = _gbtnOffset->checkedId();
    char offset = XT_OF_Open;
    switch (nid)
    {
    case 0: offset = XT_OF_Open; break;
    case 1: offset = XT_OF_Close; break;
    case 2: offset = XT_OF_CloseToday; break;
    case 3: offset = XT_OF_Auto; break;
    }

    //[1]
    tagXTTwapInputOrderField   oInputField;
    memset(&oInputField, 0, sizeof(oInputField));

    QVariant v = _cboxAccount->currentData();
    QOrderMgr* _curMgr = (QOrderMgr *)v.value<void *>();
    if(nullptr == _curMgr)
    {
        QString str = "用户未设置";
        QMessageBox::critical(nullptr, "TwapInputError", str, QMessageBox::Yes);
        return (-1);
    }

    int usertype=0;
    int broker =0;
    char szuser[32]={0};

    _curMgr->WhoIs(usertype, broker, szuser);
    oInputField.BrokerID = broker;
    strcpy(oInputField.UserID, szuser);

    char szOrderRef[13]{0};
    GetConfigModule()->GetOrderRef(szOrderRef, "147");
    strncpy(oInputField.TwapOrderID, szOrderRef, 12);

    memcpy(&oInputField.ExCode, &m_oExCode, sizeof(m_oExCode));

    //[3]
    oInputField.Direction = direction;
    oInputField.HedgeFlag = XT_HF_Speculation;
    oInputField.OffsetFlag = offset;
    oInputField.Volume = _spinVol->value();
    oInputField.TwapOrderType = XT_TWAP_NORMAL;

    oInputField.BeginTime = from.hour()*10000 + from.minute()*100 + from.second();
    oInputField.EndTime = to.hour()*10000 + to.minute()*100 + to.second();

    //[4]
    auto pTwapCtx = GetTwapOrderMgr();
    if(pTwapCtx)
    {
        auto user = stool::genUserId(broker, szuser);
        auto search = pTwapCtx->findOrderMgr(user);
        if(search)
        {
            search->DoReqTwapOrderInsert(&oInputField);
            return 1;
        }
    }

    return -1;
}







