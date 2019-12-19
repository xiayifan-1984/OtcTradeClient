#include "twapinputbox.h"
#include <string.h>
#include "configmodule.h"
#include "datamodule.h"
#include "quotemodule.h"
#include "XTCodec.h"
#include "trademodule.h"
#include "stool.h"
#include "codeinnermsg.h"
#include "twapordermgr.h"
#include <time.h>

CTwapInputBox::CTwapInputBox(QWidget *parent)
{
    memset(&m_oExCode, 0, sizeof(m_oExCode));

    _lblCode = nullptr;
    _editCode = nullptr;

    _completer = nullptr;
    _codelistModel = nullptr;

    _lblAccount = nullptr;
    _cboxAccount = nullptr;

    _lblVol = nullptr;
    _spinVol = nullptr;

    _chkOpen = nullptr;
    _chkClose = nullptr;
    _chkAuto = nullptr;

    _lblLs = nullptr;
    _chkBuy = nullptr;
    _chkSell = nullptr;

    _twapBegin = nullptr;
    _twapEnd = nullptr;

    _btnInsertTwap = nullptr;

    iniControl();
}

void CTwapInputBox::initCompleter()
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

void CTwapInputBox::setCurCommodity(tagXTInstrument &oCode)
{
    m_oExCode = oCode;
   
    //[1]获取priceTick, LotSize,  showdot, upperlimitprice,  lowerlimieprice
    tagXTNameTable oNameCode;
    QExchange* pExchange = GetDataModule()->GetExchange(oCode.ExchID);
    qDebug() << "instrument name " << oNameCode.Name;
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

void CTwapInputBox::initAccount(const char *pProductID)
{
    vector<tagTTradeUserParam>  arrUser;
    GetConfigModule()->GetTradeUserBy(pProductID, arrUser);
    qDebug() << "CTwapInputBox initAccount number " << arrUser.size();

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

void CTwapInputBox::onClickOpenChk()
{
    if(_chkOpen->isChecked())
    {
        _chkClose->setChecked(false);
        _chkAuto->setChecked(false);
        _chkCloseToday->setChecked(false);
    }
    setStartAsCurrent();
}

void CTwapInputBox::onClickCloseChk()
{
    if(_chkClose->isChecked())
    {
        _chkOpen->setChecked(false);
        _chkAuto->setChecked(false);
        _chkCloseToday->setChecked(false);
    }
    setStartAsCurrent();
}

void CTwapInputBox::onClickCloseToday()
{
    if(_chkCloseToday->isChecked())
    {
        _chkOpen->setChecked(false);
        _chkAuto->setChecked(false);
        _chkClose->setChecked(false);
    }
    setStartAsCurrent();
}

void CTwapInputBox::onClickAutoChk()
{
    if(_chkAuto->isChecked())
    {
        _chkClose->setChecked(false);
        _chkOpen->setChecked(false);
        _chkCloseToday->setChecked(false);
    }
    setStartAsCurrent();
}

void CTwapInputBox::onClickBuyChk()
{
    if(_chkBuy->isChecked())
    {
        _chkSell->setChecked(false);
    }
    setStartAsCurrent();
}

void CTwapInputBox::onClickSellChk()
{
    if(_chkSell->isChecked())
    {
        _chkBuy->setChecked(false);
    }
    setStartAsCurrent();
}

void CTwapInputBox::onCodePressReturn()
{
    QString strcode = _editCode->text();
    qDebug() << "onCodePressReturn " << strcode;
    int tid = GetCurrentThreadId();
    qDebug()<<"input linedit id " << tid;
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
    setStartAsCurrent();
}

void CTwapInputBox::onClickTwap()
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

void CTwapInputBox::iniControl()
{
    _lblCode = new QLabel();
    _lblCode->setText(tr("代码:"));
    _editCode = new QLineEdit();
   
    QHBoxLayout* h1 = new QHBoxLayout();
    h1->addWidget(_lblCode);
    h1->addWidget(_editCode);
   
    initCompleter();

    _lblAccount = new QLabel(tr("账号:"));
    _cboxAccount = new QComboBox();

    QHBoxLayout* h2 = new QHBoxLayout();
    h2->addWidget(_lblAccount);
    h2->addWidget(_cboxAccount);

    _lblOpen = new QLabel(tr("开仓"));
    _lblClose = new QLabel(tr("平仓"));
    _lblAuto = new QLabel(tr("自动"));
    _lblCloseToday = new QLabel(tr("平今"));
    _chkOpen = new QCheckBox();
    _chkClose = new QCheckBox();
    _chkAuto = new QCheckBox();
    _chkCloseToday = new QCheckBox();

    QHBoxLayout* h3 = new QHBoxLayout();
    QGridLayout* g1 = new QGridLayout();
    g1->addWidget(_lblOpen,0,1);
    g1->addWidget(_chkOpen,0,0);


    g1->addWidget(_lblClose, 0, 3);
    g1->addWidget(_chkClose, 0, 2);

    g1->addWidget(_lblCloseToday, 0, 5);
    g1->addWidget(_chkCloseToday, 0, 4);

    g1->addWidget(_lblAuto, 0, 7);
    g1->addWidget(_chkAuto, 0, 6);

    _lblBuy = new QLabel(tr("买入"));
    _lblSell = new QLabel(tr("卖出"));
    _chkBuy = new QCheckBox();
    _chkSell = new QCheckBox();

    g1->addWidget(_lblBuy, 1, 1);
    g1->addWidget(_chkBuy, 1, 0);
    g1->addWidget(_lblSell, 1, 3);
    g1->addWidget(_chkSell, 1, 2);
    h3->addLayout(g1);

    _lblVol = new QLabel(tr("数量:"));
    _spinVol = new QSpinBox();
    _spinVol->setMaximum(1000);
    _spinVol->setMinimum(0);
    QHBoxLayout* h4 = new QHBoxLayout();
    h4->addWidget(_lblVol);
    h4->addWidget(_spinVol);

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
    QHBoxLayout* h5 = new QHBoxLayout();
    h5->addLayout(g2);

    _btnInsertTwap = new QPushButton();
    _btnInsertTwap->setText("TWAP下单");
    QHBoxLayout* h6 = new QHBoxLayout();
    h6->addWidget(_btnInsertTwap);

    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->addLayout(h1);
    vlayout->addLayout(h2);
    vlayout->addLayout(h3);
    vlayout->addLayout(h4);
    vlayout->addLayout(h5);
    vlayout->addLayout(h6);

    this->setLayout(vlayout);

    QObject::connect(_chkOpen, SIGNAL(clicked()), this, SLOT(onClickOpenChk()) );
    QObject::connect(_chkClose, SIGNAL(clicked()), this, SLOT(onClickCloseChk()));
    QObject::connect(_chkCloseToday, SIGNAL(clicked()), this, SLOT(onClickCloseToday()));
    QObject::connect(_chkAuto, SIGNAL(clicked()), this, SLOT(onClickAutoChk()));
    QObject::connect(_chkSell, SIGNAL(clicked()), this, SLOT(onClickSellChk()));
    QObject::connect(_chkBuy, SIGNAL(clicked()), this, SLOT(onClickBuyChk()));
    QObject::connect(_editCode, SIGNAL(returnPressed()), this, SLOT(onCodePressReturn()) );
    QObject::connect(_btnInsertTwap, SIGNAL(clicked()), this, SLOT(onClickTwap()));
}

int CTwapInputBox::inputTwapOrder(QTime& from, QTime& to)
{
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
    
    if(_chkBuy->isChecked())
    {
        oInputField.Direction = XT_D_Buy;
    }
    else
    {
        if(_chkSell->isChecked())
        {
            oInputField.Direction = XT_D_Sell;
        }
        else
        {
            QString str = "交易方向未设置";
            QMessageBox::critical(nullptr, "TwapInputError", str, QMessageBox::Yes);
            return -1;
        }
    }

    oInputField.HedgeFlag = XT_HF_Speculation;
    if(_chkOpen->isChecked())
    {
        oInputField.OffsetFlag = XT_OF_Open;
    }
    else
    {
        if(_chkClose->isChecked())
        {
            oInputField.OffsetFlag = XT_OF_Close;
        }
        else
        {
            if(_chkAuto->isChecked())
            {
                oInputField.OffsetFlag = XT_OF_Auto;
            }
            else if(_chkCloseToday->isChecked())
            {
                oInputField.OffsetFlag = XT_OF_CloseToday;
            }
            else
            {
                QString str = "开平标志未设置";
                QMessageBox::critical(nullptr, "TwapInputError", str, QMessageBox::Yes);
                return -1;
            }
        }
    }
    oInputField.Volume = _spinVol->value();
    oInputField.TwapOrderType = XT_TWAP_NORMAL;

    oInputField.BeginTime = from.hour()*10000 + from.minute()*100 + from.second();
    oInputField.EndTime = to.hour()*10000 + to.minute()*100 + to.second();

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

void CTwapInputBox::setStartAsCurrent()
{
    _twapBegin->setTime(QTime::currentTime());
}
