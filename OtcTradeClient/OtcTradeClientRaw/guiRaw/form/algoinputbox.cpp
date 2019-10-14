#include "algoinputbox.h"
#include <string.h>
#include "configmodule.h"
#include "datamodule.h"
#include "quotemodule.h"
#include "XTCodec.h"
#include "ordermgrimpl.h"
#include "stool.h"
#include "codeinnermsg.h"
#include "algoordermgr.h"
#include <time.h>

AlgoInputBox::AlgoInputBox(QWidget *parent)
{
    memset(&m_oExCode, 0, sizeof(m_oExCode));

    _lblCode = nullptr;
    _editCode = nullptr;
    _btnSelCode = nullptr;

    _completer = nullptr;
    _codelistModel = nullptr;

    _lblAccount = nullptr;
    _cboxAccount = nullptr;

    _lblVol = nullptr;
    _spinVol = nullptr;

    _lblOffset = nullptr;
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

void AlgoInputBox::initCompleter()
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

void AlgoInputBox::setCurCommodity(tagXTInstrument &oCode)
{
    m_oExCode = oCode;
    memset(&m_oKBDetail, 0, sizeof(m_oKBDetail));

    //[1]获取priceTick, LotSize,  showdot, upperlimitprice,  lowerlimieprice
    tagXTNameTable oNameCode;
    QExchange* pExchange = GetDataModule()->GetExchange(oCode.ExchID);
    qDebug() << "instrument name " << oNameCode.Name;
    if(pExchange)
    {
        pExchange->GetOneNameTable(&oCode, &oNameCode);
        m_oKBDetail.lotsize = oNameCode.LotSize;
        m_oKBDetail.showdot = oNameCode.ShowDot;
        m_oKBDetail.multiple= oNameCode.VolumeMultiple;
        m_oKBDetail.pricetick= oNameCode.PriceTick;
    }

    tagXTSnapshot oSnap;
    int iret = GetQuoteModule()->GetSnapShot(oCode, &oSnap);
    if(iret >0)
    {
        m_oKBDetail.lowerlimitprice = oSnap.LowerLimitPrice;
        m_oKBDetail.upperlimitprice = oSnap.UpperLimitPrice;
        m_oKBDetail.defaultprice = oSnap.LastPrice;

        m_oKBDetail.dyn_valid = true;
    }
    else
    {
        m_oKBDetail.dyn_valid = false;
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

void AlgoInputBox::initAccount(const char *pProductID)
{
    vector<tagOneTradeUser>  arrUser;
    GetConfigModule()->GetTradeUserBy(pProductID, arrUser);
    qDebug() << "KBInputBox initAccount number " << arrUser.size();

    _cboxAccount->clear();
    int nsize = arrUser.size();
    for(int i = 0; i< nsize; i++)
    {
        tagOneTradeUser& o = arrUser[i];
        QOrderMgr* p = GetTradeModule()->GetMgr(o.type, o.broker, o.user);

        QString strName;
        std::string strmsg = o.aliasuser;
        strName = XTCodec::AfGbk_ToQString(strmsg);

        QVariant v = QVariant::fromValue((void *)p);
        _cboxAccount->addItem(strName, v);
    }
}

void AlgoInputBox::onClickOpenChk()
{
    if(_chkOpen->isChecked())
    {
        _chkClose->setChecked(false);
        _chkAuto->setChecked(false);
        _chkCloseToday->setChecked(false);
    }
    setStartAsCurrent();
}

void AlgoInputBox::onClickCloseChk()
{
    if(_chkClose->isChecked())
    {
        _chkOpen->setChecked(false);
        _chkAuto->setChecked(false);
        _chkCloseToday->setChecked(false);
    }
    setStartAsCurrent();
}

void AlgoInputBox::onClickCloseToday()
{
    if(_chkCloseToday->isChecked())
    {
        _chkOpen->setChecked(false);
        _chkAuto->setChecked(false);
        _chkClose->setChecked(false);
    }
    setStartAsCurrent();
}

void AlgoInputBox::onClickAutoChk()
{
    if(_chkAuto->isChecked())
    {
        _chkClose->setChecked(false);
        _chkOpen->setChecked(false);
        _chkCloseToday->setChecked(false);
    }
    setStartAsCurrent();
}

void AlgoInputBox::onClickBuyChk()
{
    if(_chkBuy->isChecked())
    {
        _chkSell->setChecked(false);
    }
    setStartAsCurrent();
}

void AlgoInputBox::onClickSellChk()
{
    if(_chkSell->isChecked())
    {
        _chkBuy->setChecked(false);
    }
    setStartAsCurrent();
}

void AlgoInputBox::onCodePressReturn()
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

void AlgoInputBox::onClickTwap()
{
    if(m_oExCode.ExchID <=0)
    {
        QString str = "交易品种未设置";
        QMessageBox::critical(nullptr, "AlgoInputError", str, QMessageBox::Yes);
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
        QMessageBox::critical(nullptr, "AlgoInputError", str, QMessageBox::Yes);
        return;
    }
    inputTwapOrder(begin, end);
}

void AlgoInputBox::iniControl()
{
    _lblCode = new QLabel();
    _lblCode->setText(tr("Instrument:"));

    _editCode = new QLineEdit();
    _btnSelCode = new QPushButton();

    _btnSelCode->setText(tr("..."));

    QHBoxLayout* h1 = new QHBoxLayout();
    h1->addWidget(_lblCode);
    h1->addWidget(_editCode);
   // h1->addWidget(_btnSelCode);
    initCompleter();

    _lblAccount = new QLabel(tr("User:"));
    _cboxAccount = new QComboBox();

    QHBoxLayout* h2 = new QHBoxLayout();
    h2->addWidget(_lblAccount);
    h2->addWidget(_cboxAccount);

    _lblOffset = new QLabel(tr("OpenOrClose:"));
    _lblOpen = new QLabel(tr("Open"));
    _lblClose = new QLabel(tr("Close"));
    _lblAuto = new QLabel(tr("Auto"));
    _lblCloseToday = new QLabel(tr("CToday"));
    _chkOpen = new QCheckBox();
    _chkClose = new QCheckBox();
    _chkAuto = new QCheckBox();
    _chkCloseToday = new QCheckBox();

    QHBoxLayout* h3 = new QHBoxLayout();
    //h3->addWidget(_lblOffset);
    QGridLayout* g1 = new QGridLayout();
    g1->addWidget(_lblOpen,0,1);
    g1->addWidget(_chkOpen,0,0);


    g1->addWidget(_lblClose, 0, 3);
    g1->addWidget(_chkClose, 0, 2);

    g1->addWidget(_lblCloseToday, 0, 5);
    g1->addWidget(_chkCloseToday, 0, 4);

    g1->addWidget(_lblAuto, 0, 7);
    g1->addWidget(_chkAuto, 0, 6);

    _lblBuy = new QLabel(tr("Buy"));
    _lblSell = new QLabel(tr("Sell"));
    _chkBuy = new QCheckBox();
    _chkSell = new QCheckBox();

    g1->addWidget(_lblBuy, 1, 1);
    g1->addWidget(_chkBuy, 1, 0);
    g1->addWidget(_lblSell, 1, 3);
    g1->addWidget(_chkSell, 1, 2);
    h3->addLayout(g1);

    _lblVol = new QLabel(tr("Volumn:"));
    _spinVol = new QSpinBox();
    _spinVol->setMaximum(1000);
    _spinVol->setMinimum(0);
    QHBoxLayout* h4 = new QHBoxLayout();
    h4->addWidget(_lblVol);
    h4->addWidget(_spinVol);

    _lblStart = new QLabel(tr("Start:"));
    _lblEnd = new QLabel(tr("End:"));
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
    _btnInsertTwap->setText("TWAP");
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

int AlgoInputBox::inputTwapOrder(QTime& from, QTime& to)
{
    tagXTAlgoOrderField order;
    memset(&order, 0, sizeof(order));

    QVariant v = _cboxAccount->currentData();
    QOrderMgr* _curMgr = (QOrderMgr *)v.value<void *>();
    if(nullptr == _curMgr)
    {
        QString str = "用户未设置";
        QMessageBox::critical(nullptr, "AlgoInputError", str, QMessageBox::Yes);
        return (-1);
    }

    int usertype=0;
    int broker =0;
    char szuser[32]={0};

    _curMgr->WhoIs(usertype, broker, szuser);
    order.BrokerID = broker;
    strcpy(order.UserID, szuser);

    memcpy(&order.ExCode, &m_oExCode, sizeof(m_oExCode));
    order.Volumn = _spinVol->value();
    if(_chkBuy->isChecked())
    {
        order.Direction = XT_D_Buy;
    }
    else
    {
        if(_chkSell->isChecked())
        {
            order.Direction = XT_D_Sell;
        }
        else
        {
            QString str = "交易方向未设置";
            QMessageBox::critical(nullptr, "AlgoInputError", str, QMessageBox::Yes);
            return -1;
        }
    }

    if(_chkOpen->isChecked())
    {
        order.Offset = XT_OF_Open;
    }
    else
    {
        if(_chkClose->isChecked())
        {
            order.Offset = XT_OF_Close;
        }
        else
        {
            if(_chkAuto->isChecked())
            {
                order.Offset = XT_OF_Auto;
            }
            else if(_chkCloseToday->isChecked())
            {
                order.Offset = XT_OF_CloseToday;
            }
            else
            {
                QString str = "开平标志未设置";
                QMessageBox::critical(nullptr, "AlgoInputError", str, QMessageBox::Yes);
                return -1;
            }
        }
    }

    order.AlgoOrderType = XT_ALGO_TWAP;
    order.AlgoOrderStatus = '\0';

    char szOrderRef[13]{0};
    GetConfigModule()->GetOrderRef(szOrderRef, "147");
    strncpy(order.AlgoOrderID, szOrderRef, 12);

    QTime s = from;
    QTime e = to;

    auto f = [](QTime& t){
      if(t >= QTime::fromString("00:00:00") && t <= QTime::fromString("03:00:00")
         && QTime::currentTime() >= QTime::fromString("00:00:00") && QTime::currentTime() <= QTime::fromString("03:00:00"))
      {
          return QDateTime::currentDateTime();
      }

      if(t >= QTime::fromString("00:00:00") && t <= QTime::fromString("03:00:00")
         && QTime::currentTime() >= QTime::fromString("03:00:00"))
      {
          auto today = QDateTime::currentDateTime();
          auto tomorrow = today.addDays(1);
          return tomorrow;
      }
      return QDateTime::currentDateTime();
    };

    QDateTime start = f(s);
    start.setTime(s);

    QDateTime end = f(e);
    end.setTime(e);

    order.StartTime = start.toTime_t();
    order.EndTime = end.toTime_t();

    if(order.StartTime >= order.EndTime)
    {
        QString str = "开始时间大于结束时间";
        QMessageBox::critical(nullptr, "AlgoInputError", str, QMessageBox::Yes);
        return -1;
    }

    auto u = localtime((time_t*)&(order.StartTime));
    auto pTwapCtx = GetAlgoOrderMgr();
    if(pTwapCtx)
    {
        auto user = stool::genUserId(broker, szuser);
        auto search = pTwapCtx->findOrderMgr(user);
        if(search)
        {
            search->insertAlgoOrder(&order);
            return 1;
        }
    }
    return -1;
}

void AlgoInputBox::setStartAsCurrent()
{
    _twapBegin->setTime(QTime::currentTime());
}
