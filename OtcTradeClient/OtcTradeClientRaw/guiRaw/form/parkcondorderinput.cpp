
#include "parkcondorderinput.h"
#include "quotectrl.h"
#include "selectcommodity.h"
#include "datamodule.h"
#include "quotemodule.h"
#include "XTCodec.h"
#include "configmodule.h"
#include "trademodule.h"
#include "eventcentermodule.h"
#include <memory>
#include <iostream>
#include "XTCodec.h"
#include "codeinnermsg.h"
#include "stool.h"
#include "parkordermgr.h"

extern QOrderMgr* getOrderMgrbyInstrument(tagXTInstrument&);
extern int posDirectionSign(XTPosiDirectionType);
extern QMdiArea * G_midArea;



ParkCondOrderInput::ParkCondOrderInput(QWidget* parent /*= nullptr*/)
{
    setAttribute(Qt::WA_DeleteOnClose);
    memset(&m_oExCode, 0, sizeof(tagXTInstrument));
    memset(&m_oKBDetail, 0, sizeof(m_oKBDetail));
    m_isAutoOpenClose = false;
    m_condParams = nullptr;

    initControls();

    configControls();

}

void ParkCondOrderInput::inputCommodity(tagXTInstrument &oCode)
{
    setCurCommodity(oCode);
}

void            ParkCondOrderInput::initControls()
{
    //右边
    QGridLayout* g1 = new QGridLayout();
    {
        _lblCode = new QLabel(tr("代码:"));
        _editCode = new QLineEdit();
        _btnSelCode = new QPushButton(tr("..."));

        g1->addWidget(_lblCode, 0, 0);

        QHBoxLayout* h1 = new QHBoxLayout();
        h1->addWidget(_editCode);
        h1->addWidget(_btnSelCode);
        g1->addLayout(h1, 0, 1, 1, 2);

        initCompleter();

    }
    {
        _lblAccount = new QLabel(tr("账号:"));
        _cboxAccount = new QComboBox();
        _chkHedge = new QCheckBox(tr("套保"));
        g1->addWidget(_lblAccount, 1, 0);
        g1->addWidget(_cboxAccount, 1,1);
        g1->addWidget(_chkHedge, 1, 2);
    }

    {
        QHBoxLayout* h1 = new QHBoxLayout();
        _spinVol = new QSpinBox();
        _lblVol = new QLabel(tr("(手)"));
        h1->addWidget(_spinVol, 4);
        h1->addWidget(_lblVol, 1);

        g1->addLayout(h1, 2, 0, 1, 2);
       // g1->addWidget(_chkAutoKP, 2, 2);

    }

    {
        QHBoxLayout* h2 = new QHBoxLayout();
        _spinPrice= new QDoubleSpinBox();
        _lblPrice= new QLabel(tr("(元)"));
        h2->addWidget(_spinPrice, 4);
        h2->addWidget(_lblPrice, 1);

        _chkAny = new QCheckBox(tr("市价"));
        g1->addLayout(h2, 3, 0, 1, 2);
        g1->addWidget(_chkAny, 2, 2);

        _chCloseTodayOrYes = new QCheckBox(tr("平今"));
        g1->addLayout(h2, 3, 0, 1, 2);
        g1->addWidget(_chCloseTodayOrYes, 3, 2);

    }

    {
        //QHBoxLayout* h3 = new QHBoxLayout();
        _chkAutoKP = new QCheckBox(tr("自动开平"));
        _chkParkedOrder = new QCheckBox(tr("预埋单"));
        _chkCondOrder = new QCheckBox(tr("条件单"));

        g1->addWidget(_chkAutoKP, 5,2);
        g1->addWidget(_chkParkedOrder,5,0);
        g1->addWidget(_chkCondOrder,5,1);
        //h3->setSpacing(2);
       // g1->addLayout(h3, 4, 1,2,2);
    }

    QGridLayout* g4 = new QGridLayout();
    {

        _btnBuyOpen = new QPushButton(tr("买(开)"));
        _btnBuyClose = new QPushButton(tr("买(平)"));
        _btnSellOpen = new QPushButton(tr("卖(开)"));
        _btnSellClose = new QPushButton(tr("卖(平)"));
        g4->addWidget(_btnBuyOpen, 0, 0);
        g4->addWidget(_btnSellOpen, 0, 1);
        g4->addWidget(_btnBuyClose, 1, 0);
        g4->addWidget(_btnSellClose, 1, 1);
    }
    QHBoxLayout* h3 = new QHBoxLayout();
    h3->addLayout(g4);

    _btnCondInsert = new QPushButton(tr(""));
    _btnCondInsert->setDisabled(true);
    h3->addWidget(_btnCondInsert);

    QGridLayout* g5 = new QGridLayout();
    {
        m_condParams = new CondOrderBox();
        g5->addWidget(m_condParams);
    }

    QVBoxLayout* v1 = new QVBoxLayout();
    {
        v1->addLayout(g1);
        v1->addLayout(h3);
        v1->addLayout(g5);
    }

    //左边
    _drawQuote = new CQuoteCtrl();
    QHBoxLayout* mainlayout = new QHBoxLayout();
    mainlayout->addWidget(_drawQuote, 1);
    mainlayout->addLayout(v1, 1);

    this->setLayout(mainlayout);

    //[2]消息
    QObject::connect(_btnSelCode, SIGNAL(clicked()), this, SLOT(onCodeBtnPushed()) );
    QObject::connect(_drawQuote, SIGNAL(quoteClick(int)), this, SLOT(onQuoteClick(int)) );
    QObject::connect(_editCode, SIGNAL(returnPressed()), this, SLOT(onCodePressReturn()) );
    QObject::connect(_chkAutoKP, SIGNAL(clicked()), this, SLOT(onAutoChkClicked()));
    QObject::connect(_chkCondOrder, SIGNAL(clicked()), this, SLOT(onCondChkClicked()));
    QObject::connect(_chkParkedOrder, SIGNAL(clicked()), this, SLOT(onParkedChkClicked()));
    QObject::connect(_chCloseTodayOrYes, SIGNAL(clicked()), this, SLOT(onCloseTodayClick()));

    //[3]代码框，价格，数量框，设置为粗体
    const QFont oldfont = this->font();
    QFont newfont(oldfont);
    newfont.setBold(true);
    _editCode->setFont(newfont);
    _spinVol->setFont(newfont);
    _spinPrice->setFont(newfont);

    //[4]价格，数量 靠右侧显示
    _spinVol->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    _spinPrice->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    //[5]消息
    QObject::connect(_btnBuyOpen, SIGNAL(clicked()), this,  SLOT(on_buyopenbtn_clicked_or_autolong()) );
    QObject::connect(_btnBuyClose, SIGNAL(clicked()), this,  SLOT(on_buyclosebtn_clicked()) );
    QObject::connect(_btnSellOpen, SIGNAL(clicked()), this,  SLOT(on_sellopenbtn_clicked_or_autoshort()) );
    QObject::connect(_btnSellClose, SIGNAL(clicked()), this,  SLOT(on_sellclosebtn_clicked()) );

    QObject::connect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onOrderEvent(OrderEventArgs*)) );

}

void    ParkCondOrderInput::configControls()
{
    //[0]默认选择： 默认为非套保，默认为非自动开平，默认为非市价，
    _chkHedge->setChecked(false);
    _chkAutoKP->setChecked(false);
    _chkAny->setChecked(false);

    //[1]默认值
    _editCode->setPlaceholderText(tr("IF1905"));

    //[2]spinVol默认值
    _spinVol->setMinimum(1);
    _spinVol->setMaximum(100);
    _spinVol->setSingleStep(1);

    //[3]spinPrice默认值
    _spinPrice->setMinimum(0.0);
    _spinPrice->setMaximum(10000);
    _spinPrice->setSingleStep(0.01);
    _spinPrice->setDecimals(2);

    //[4]外观
    _btnSelCode->setProperty("name", "selcode");
    _btnBuyClose->setProperty("name", "buy");
    _btnBuyOpen->setProperty("name", "buy");
    _btnSellClose->setProperty("name", "sell");
    _btnSellOpen->setProperty("name", "sell");

    _btnBuyClose->setStyleSheet("background: rgb(250,0,50)");
    _btnBuyOpen->setStyleSheet("background: rgb(250,0,50)");

    _btnSellClose->setStyleSheet("background: rgb(50,255,0)");
    _btnSellOpen->setStyleSheet("background: rgb(50,255,0)");

    _btnCondInsert->setFixedHeight(50);
    _btnCondInsert->setStyleSheet("background: rgb(105,200,255)");

    this->setProperty("name","ParkCondOrderInput");

    _chkAutoKP->setChecked(false);
    _chkCondOrder->setChecked(false);
    _chkParkedOrder->setChecked(false);
}

//选择代码对话框，返回之后
void    ParkCondOrderInput::onCodeBtnPushed()
{
    SelectCommodity dlg;
    if(dlg.exec() == QDialog::Accepted)
    {
        tagXTInstrument oCode;
        dlg.getSelectCommodity(&oCode);

        this->setCurCommodity(oCode);
    }
}

void    ParkCondOrderInput::onQuoteClick(int idx)
{
    double dprice =0.0;
    _drawQuote->getClickPrice(idx, dprice);

    //如果未赋值初始行情
    if(!m_oKBDetail.dyn_valid)
    {
        tagXTSnapshot oSnap;
        int iret = GetQuoteModule()->GetSnapShot(m_oExCode, &oSnap);
        if(iret >0)
        {
            m_oKBDetail.lowerlimitprice = oSnap.LowerLimitPrice;
            m_oKBDetail.upperlimitprice = oSnap.UpperLimitPrice;
            m_oKBDetail.defaultprice = oSnap.LastPrice;

            m_oKBDetail.dyn_valid = true;
        }

        //[3]_spinPrice 单位是元， 最小是跌停价，最大是涨停价， 步长为tick, 如果是输入，需要对齐为tick
        if(m_oKBDetail.dyn_valid)
        {
            _spinPrice->setMinimum(m_oKBDetail.lowerlimitprice);
            _spinPrice->setMaximum(m_oKBDetail.upperlimitprice);

            _spinPrice->setSingleStep(m_oKBDetail.pricetick);
            _spinPrice->setDecimals(m_oKBDetail.showdot);
        }

    }

    //修正，如果低于跌停价
    if(dprice < m_oKBDetail.lowerlimitprice)
    {
        dprice = m_oKBDetail.defaultprice;
    }

    _spinPrice->setValue(dprice);
}

void      ParkCondOrderInput::setCurCommodity(tagXTInstrument& oCode)
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
    _drawQuote->setCurCommodity(oCode);

    //[3]_spinPrice 单位是元， 最小是跌停价，最大是涨停价， 步长为tick, 如果是输入，需要对齐为tick
    if(m_oKBDetail.dyn_valid)
    {
        _spinPrice->setMinimum(m_oKBDetail.lowerlimitprice);
        _spinPrice->setMaximum(m_oKBDetail.upperlimitprice);

        _spinPrice->setSingleStep(m_oKBDetail.pricetick);
        _spinPrice->setDecimals(m_oKBDetail.showdot);
    }
    else
    {
        _spinPrice->setMinimum(0);
        _spinPrice->setMaximum(10000);

        _spinPrice->setSingleStep(m_oKBDetail.pricetick);
        _spinPrice->setDecimals(m_oKBDetail.showdot);
    }

    //[4]设置当前最新价,如果没有最新价，则设置为0
    if(m_oKBDetail.dyn_valid)
    {
        _spinPrice->setValue(m_oKBDetail.defaultprice);
    }
    else
    {
        _spinPrice->setValue(0);
    }

    //[5]数量初始化为1
    _spinVol->setValue(1);

    //[6]初始化账号ComboBox
    initAccount(oNameCode.ProductID);
    if(_chkCondOrder->isChecked() && m_condParams != nullptr)
    {
        m_condParams->setCurCommodity(m_oExCode, (void*)&m_oKBDetail);
    }
}

//编辑框输入，回车
void            ParkCondOrderInput::onCodePressReturn()
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
}

void    ParkCondOrderInput::initAccount(const char* pProductID)
{
    vector<tagOneTradeUser>  arrUser;
    GetConfigModule()->GetTradeUserBy(pProductID, arrUser);
    qDebug() << "ParkCondOrderInput initAccount number " << arrUser.size();

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

void        ParkCondOrderInput::initCompleter()
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

void   ParkCondOrderInput::on_buyopenbtn_clicked_or_autolong()    //买入开仓
{
    if(_chkCondOrder->isChecked())
    {
        m_condParams->setBsAndOffset(BUY_SELL::BUY, _chkAutoKP->isChecked()?OPEN_CLOSE::AUTO : OPEN_CLOSE::OPEN);
        return;
    }
    if(_chkParkedOrder->isChecked())
    {
        qDebug() << "is auto kp = " << _chkAutoKP->isChecked();
        auto offset = _chkAutoKP->isChecked() ? XT_OF_Auto : XT_OF_Open;
        insertParkOrder(XT_D_Buy, offset, _spinVol->value());
        return;
    }
    int iret = inputOrderCommon(XT_D_Buy, XT_OF_Open, _spinVol->value());
    if(iret >0)
    {
        return;
    }
    QString strmsg;
    switch(iret)
    {
    case (-1):strmsg="请输入有效的代码";break;
    case (-2):strmsg="无法获取基础行情";break;
    case (-3):strmsg="没有合适的交易账号";break;
    case (-4):strmsg="交易数量是无效的";break;
    case (-5):strmsg="限价条件下，价格超出了涨跌停范围";break;
    case (-6):strmsg="无法获取有效的下单通道,请确认委托模块是否开启";break;
    default:strmsg ="未知错误，请与开发人员联系";break;
    }

    QMessageBox::critical(nullptr, "下单校验错误", strmsg, QMessageBox::Yes);
}

void   ParkCondOrderInput::on_buyclosebtn_clicked()   //买入平仓
{
    if(_chkCondOrder->isChecked())
    {
        m_condParams->setBsAndOffset(BUY_SELL::BUY, OPEN_CLOSE::CLOSE);
        return;
    }
    qDebug()<<"buy close is clicked";
    auto vol = _spinVol->value();
    auto pos = getShortOrLongPostionsBy(m_oExCode, false);
    qDebug() << "close vol = " << vol << ", today short position = " << pos.second;
    closePostion(XT_D_Buy, vol, pos.second);
}

void   ParkCondOrderInput::on_sellopenbtn_clicked_or_autoshort()   //卖出开仓
{
    if(_chkCondOrder->isChecked())
    {
        m_condParams->setBsAndOffset(BUY_SELL::SELL, _chkAutoKP->isChecked()?OPEN_CLOSE::AUTO : OPEN_CLOSE::OPEN);
        return;
    }
    qDebug()<<"sell open is clicked";
    int iret = inputOrderCommon(XT_D_Sell, XT_OF_Open, _spinVol->value());
    if(iret >0)
    {
        return;
    }
}

void   ParkCondOrderInput::on_sellclosebtn_clicked()  //卖出平仓
{
    if(_chkCondOrder->isChecked())
    {
        m_condParams->setBsAndOffset(BUY_SELL::SELL, OPEN_CLOSE::CLOSE);
        return;
    }
    qDebug()<<"sell close is clicked";

    auto vol = _spinVol->value();
    auto pos = getShortOrLongPostionsBy(m_oExCode, true);
    closePostion(XT_D_Sell, vol, pos.second);
}

int    ParkCondOrderInput::inputOrderCommon(char  direction, char  offsetflag, int volume)
{
    //[1]检查代码
    if(m_oExCode.ExchID ==0)
    {
        return (-1);
    }
    if(!m_oKBDetail.dyn_valid)
    {
        return (-2);
    }

    //[2]检查账号
    if(_cboxAccount->count() <=0)
    {
        return (-3);
    }

    //[3]数量检查
    int nval = volume;
    if(nval <=0)
    {
        return (-4);
    }

    //[4]价格检查(市价限价)
    const double dprice = _spinPrice->value();
    if(!_chkAny->isChecked())
    {
        if(dprice < m_oKBDetail.lowerlimitprice || dprice > m_oKBDetail.upperlimitprice)
        {
            return (-5);
        }
    }

    QVariant v = _cboxAccount->currentData();
    QOrderMgr* _curMgr = (QOrderMgr *)v.value<void *>();
    if(nullptr == _curMgr)
    {
        return (-6);
    }

    tagXTInputOrderField oField;
    memset(&oField, 0, sizeof(oField));
    {
        int usertype=0;
        int broker =0;
        char szuser[32]={0};

        _curMgr->WhoIs(usertype, broker, szuser);
        oField.BrokerID = broker;
        strcpy(oField.UserID, szuser);
        oField.ExCode = m_oExCode;
        oField.Volume = nval;

        if(_chkAny->isChecked() )
        {
            oField.PriceType = XT_OPT_AnyPrice;
            oField.LimitPrice =0;
        }
        else
        {
            oField.PriceType = XT_OPT_LimitPrice;
            oField.LimitPrice = dprice;
        }

        if(_chkHedge->isChecked())
        {
            oField.HedgeFlag[0] = XT_HF_Hedge;
            oField.HedgeFlag[1] = XT_HF_Hedge;
        }
        else
        {
            oField.HedgeFlag[0] = XT_HF_Speculation;
            oField.HedgeFlag[1] = XT_HF_Speculation;
        }

        oField.Direction = direction;
        oField.OffsetFlag[0] = offsetflag;
        oField.OffsetFlag[1] = offsetflag;

        char szOrderRef[20]={0};
        GetConfigModule()->GetOrderRef(szOrderRef, "117");
        strncpy(oField.OrderRef, szOrderRef, 12);
    }
    _curMgr->InsertOrder(&oField);

    return 1;
}

void            ParkCondOrderInput::onOrderEvent(OrderEventArgs* e)
{
    if(e->subevent == ORDER_EVENT_FAIL)
    {
        std::cout << e->reason ;
        if (_strnicmp(e->orderref+9, "117", 3) ==0 )
        {
            QString strmsg;
            //QString str = QString::fromLocal8Bit(e->reason);
            string t(e->reason);
            QString str = XTCodec::AfGbk_ToQString(t);
            strmsg.sprintf("下单[%s]失败, ", e->orderref);
            strmsg.append(str);
            QMessageBox::critical(nullptr, "下单失败", strmsg, QMessageBox::Yes);
        }
    }
}

void ParkCondOrderInput::onAutoChkClicked()
{
    if(_chkAutoKP->isChecked())
    {
        m_isAutoOpenClose = true;
        qDebug() << "is Checked " << m_isAutoOpenClose;
        _btnBuyOpen->setText("多");
        _btnBuyClose->setDisabled(true);
        _btnBuyClose->setText(tr(""));
        _btnSellOpen->setText("空");
        _btnSellClose->setDisabled(true);
        _btnSellClose->setText(tr(""));

        _chCloseTodayOrYes->setChecked(false);
        _btnBuyClose->setVisible(false);
        _btnSellClose->setVisible(false);
    }
    else
    {
        m_isAutoOpenClose = false;
        qDebug() << "is Checked " << m_isAutoOpenClose;
        _btnBuyOpen->setText(tr("买(开)"));
        _btnBuyClose->setDisabled(false);
        _btnBuyClose->setText(tr("买(平)"));
        _btnSellOpen->setText(tr("卖(开)"));
        _btnSellClose->setDisabled(false);
        _btnSellClose->setText(tr("卖(平)"));

        _chCloseTodayOrYes->setDisabled(false);
        _btnBuyClose->setVisible(true);
        _btnSellClose->setVisible(true);
    }
}

void ParkCondOrderInput::onCondChkClicked()
{
    if(_chkCondOrder->isChecked())
    {
        _chkParkedOrder->setChecked(false);
        _btnCondInsert->setText(tr("条件下单"));
        _btnCondInsert->setDisabled(false);

        if(m_condParams != nullptr)
        {
            m_condParams->setCurCommodity(m_oExCode, (void*)&m_oKBDetail);
        }
    }
    else
    {
        _chkParkedOrder->setChecked(false);
        _btnCondInsert->setDisabled(true);
        _btnCondInsert->setText(tr(""));
        m_condParams->reset();
    }
}

void ParkCondOrderInput::onParkedChkClicked()
{
    if(_chkParkedOrder->isChecked())
    {
        _chkCondOrder->setChecked(false);
        _btnCondInsert->setDisabled(true);
        _btnCondInsert->setText(tr(""));
        if(m_condParams)
        {
            m_condParams->reset();
        }
    }
}

void ParkCondOrderInput::onCondBtnClick()
{

}

void ParkCondOrderInput::onCloseTodayClick()
{
    if(_chCloseTodayOrYes->isChecked())
    {
        _chkAutoKP->setChecked(false);
        _btnBuyOpen->setText(tr("买(开)"));
        _btnBuyClose->setDisabled(false);
        _btnBuyClose->setText(tr("买(平)"));
        _btnSellOpen->setText(tr("卖(开)"));
        _btnSellClose->setDisabled(false);
        _btnSellClose->setText(tr("卖(平)"));

        _btnBuyClose->setVisible(true);
        _btnSellClose->setVisible(true);
    }
}

void ParkCondOrderInput::closeEvent(QCloseEvent *event)
{
    event->accept();
}

std::pair<int, int> ParkCondOrderInput::getShortOrLongPostionsBy(tagXTInstrument &cur_Code, bool isLong)
{
    int position  = 0;
    int todayPostion = 0;
    auto pMgr = getOrderMgrbyInstrument(cur_Code);
    if(!pMgr) return {0,0};
    vector<char> directionsVec;
    if(isLong)
    {
        directionsVec.push_back(XT_PD_Long);
    }
    else
    {
        directionsVec.push_back(XT_PD_Short);
    }
    vector<char> hedgeflagsVec{XT_HF_Others, XT_HF_Speculation, XT_HF_Arbitrage, XT_HF_Hedge, XT_HF_Covered, XT_HF_MarketMaker};

    //update position
    for(auto dir:directionsVec)
    {
        for(auto hedgeflag:hedgeflagsVec)
        {
            tagXTInvestorPositionField temp;
            if(1 == pMgr->GetOnePosition(cur_Code, dir, hedgeflag, temp) and temp.Position>=0)
            {
                position += temp.Position;
                todayPostion += temp.TodayPosition;
            }
        }
    }
    qDebug() << "position = " << position << " todayPostion = " << todayPostion;
    return {position, todayPostion};
}

void ParkCondOrderInput::closePostion(char direction, int vol, int todayPosition)
{
    int iret;
    qDebug() << "close vol = " << vol << ", today position = " << todayPosition;
    XTOffsetFlagType offset = _chCloseTodayOrYes->isChecked() ? XT_OF_CloseToday:XT_OF_Close;
    iret = inputOrderCommon(direction, offset, vol);
}

int ParkCondOrderInput::insertParkOrder(char direction, char offsetflag, int volume)
{
    //[1]检查代码
    if(m_oExCode.ExchID ==0)
    {
        return (-1);
    }
  /*  if(!m_oKBDetail.dyn_valid)
    {
        return (-2);
    }*/

    //[2]检查账号
    if(_cboxAccount->count() <=0)
    {
        return (-3);
    }

    //[3]数量检查
    int nval = volume;
    if(nval <=0)
    {
        return (-4);
    }

    //[4]价格检查(市价限价)
    const double dprice = _spinPrice->value();
    if(!_chkAny->isChecked())
    {
        if(dprice < m_oKBDetail.lowerlimitprice || dprice > m_oKBDetail.upperlimitprice)
        {
            return (-5);
        }
    }

    QVariant v = _cboxAccount->currentData();
    auto _curMgr = (QOrderMgr *)v.value<void *>();
    if(nullptr == _curMgr)
    {
        return (-6);
    }

    int type;
    int broker;
    char  userid[32]{0};
    _curMgr->WhoIs(type, broker, userid);

   // tagXTInputOrderField oField;
    tagXTParkedOrderField oField;
    memset(&oField, 0, sizeof(oField));
    {
        int usertype=0;
        int broker =0;
        char szuser[32]={0};

        _curMgr->WhoIs(usertype, broker, szuser);
        oField.BrokerID = broker;
        strcpy(oField.UserID, szuser);
        oField.ExCode = m_oExCode;
        oField.Volume = nval;

        if(_chkAny->isChecked() )
        {
            oField.PriceType = XT_OPT_AnyPrice;
            oField.LimitPrice =0;
        }
        else
        {
            oField.PriceType = XT_OPT_LimitPrice;
            oField.LimitPrice = dprice;
        }

        if(_chkHedge->isChecked())
        {
            oField.HedgeFlag[0] = XT_HF_Hedge;
            oField.HedgeFlag[1] = XT_HF_Hedge;
        }
        else
        {
            oField.HedgeFlag[0] = XT_HF_Speculation;
            oField.HedgeFlag[1] = XT_HF_Speculation;
        }

        oField.Direction = direction;
        oField.OffsetFlag[0] = offsetflag;
        oField.OffsetFlag[1] = offsetflag;

        oField.ParkedType = XT_CC_ParkedOrder;
        oField.ParkedStatus = XT_PAOS_NotSend;
        oField.StopPrice = dprice;

        char szOrderRef[13]{0};
        int sNo = qrand()%1000;
        GetConfigModule()->GetOrderRef(szOrderRef, (char*)(QString::number(sNo).data()));
        strncpy(oField.ParkedOrderID, szOrderRef, 12);
    }
    //_curMgr->InsertOrder(&oField);
    auto pPark = GetParkedOrderMgr();
    if(pPark)
    {
        auto strkey = stool::genParkUserId(broker, userid);
        auto pUser = pPark->findMgrByUser(strkey);
        if(pUser)
        {
            pUser->reqParkedOrderInsert(&oField);
        }
    }
    return 1;
}

