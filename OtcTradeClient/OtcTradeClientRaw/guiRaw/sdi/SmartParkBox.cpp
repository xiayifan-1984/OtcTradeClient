
#include "SmartParkBox.h"
#include "quotectrl.h"
#include "datamodule.h"
#include "quotemodule.h"
#include "XTCodec.h"
#include "configmodule.h"
#include "trademodule.h"
#include "eventcentermodule.h"
#include <memory>
#include <iostream>

#include "SmartParkDlg.h"

CSmartParkBox::CSmartParkBox(QWidget* parent /*= nullptr*/)
{
    setAttribute(Qt::WA_DeleteOnClose);
    memset(&m_oExCode, 0, sizeof(tagXTInstrument));
    memset(&m_oKBDetail, 0, sizeof(m_oKBDetail));

    initControls();

    configControls();
}

CSmartParkBox::~CSmartParkBox()
{

}


void            CSmartParkBox::initControls()
{
    //一个GridLayout
    QGridLayout* g1 = new QGridLayout();

    //第一行
    {
        _lblCode = new QLabel(tr("合约:"));
        _editCode = new QLineEdit();

        QVBoxLayout* v1 = new QVBoxLayout();
        _chkArbitrage = new QCheckBox(tr("套利"));
        _chkHedge = new QCheckBox(tr("套保"));
        v1->addWidget(_chkArbitrage);
        v1->addWidget(_chkHedge);
        v1->setSpacing(1);

        //布局
        g1->addWidget(_lblCode, 0, 0);
        g1->addWidget(_editCode, 0, 1);
        g1->addLayout(v1, 0, 2);

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
        _lblPrice = new QLabel(tr("价格:"));
        _spinPrice= new QDoubleSpinBox();
        g1->addWidget(_lblPrice, 5, 0);
        g1->addWidget(_spinPrice, 5, 1);
    }

    //第七行
    {
        _btnInput = new QPushButton(tr("下单"));

        QVBoxLayout* v2 = new QVBoxLayout();
        _btnCancel = new QPushButton(tr("取消"));
        _btnPark = new QPushButton(tr("条件"));

        v2->addWidget(_btnCancel);
        v2->addWidget(_btnPark);
        v2->setSpacing(1);

        g1->addWidget(_btnInput, 6, 1);
        g1->addLayout(v2, 6, 2);
    }

    //总布局
    _drawQuote = new CQuoteCtrl();
    QHBoxLayout* sublayout = new QHBoxLayout();
    sublayout->addWidget(_drawQuote);
    sublayout->addLayout(g1);
    sublayout->addStretch();

    QVBoxLayout* mainlayout = new QVBoxLayout();
    mainlayout->addLayout(sublayout);
    mainlayout->addStretch();

    this->setLayout(mainlayout);

    //[2]消息
    QObject::connect(_drawQuote, SIGNAL(quoteClick(int)), this, SLOT(onQuoteClick(int)) );
    QObject::connect(_editCode, SIGNAL(returnPressed()), this, SLOT(onCodePressReturn()) );

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
    QObject::connect(_btnInput, SIGNAL(clicked()), this,  SLOT(on_inputbtn_clicked()) );
    QObject::connect(_btnCancel, SIGNAL(clicked()), this, SLOT(on_cancelbtn_clicked()) );
    QObject::connect(_btnPark, SIGNAL(clicked()), this, SLOT(on_parkbtn_clicked()) );

    QObject::connect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onOrderEvent(OrderEventArgs*)) );

}

void    CSmartParkBox::configControls()
{
    //[0]默认选择： 默认为非套保
    _chkHedge->setChecked(false);
    _chkArbitrage->setChecked(false);
    _editCode->setPlaceholderText(tr("IF1912"));

    //[1]默认值
    _rbtnBuy->setChecked(true);
    _rbtnOpen->setChecked(true);

    //[2]spinVol默认值
    _spinVol->setMinimum(1);
    _spinVol->setMaximum(1000);
    _spinVol->setSingleStep(1);

    //[3]spinPrice默认值
    _spinPrice->setMinimum(0.0);
    _spinPrice->setMaximum(10000);
    _spinPrice->setSingleStep(0.01);
    _spinPrice->setDecimals(2);

    //[4]外观
    _rbtnBuy->setProperty("name", "buyradio");
    _rbtnOpen->setProperty("name", "openradio");
    _btnInput->setProperty("base", "input");
    _btnInput->setProperty("name", "buy");
    _btnCancel->setProperty("name", "cancel");
    _btnPark->setProperty("name", "park");
    this->setProperty("name","smartparkbox");
}


void     CSmartParkBox::initCompleter()
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
    for(size_t i=0; i< arrExch.size(); i++)
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

//编辑框输入，回车
void            CSmartParkBox::onCodePressReturn()
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


void    CSmartParkBox::onQuoteClick(int idx)
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


void      CSmartParkBox::setCurCommodity(tagXTInstrument& oCode)
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
}

void    CSmartParkBox::initAccount(const char* pProductID)
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

void    CSmartParkBox::onOrderEvent(OrderEventArgs* e)
{
    if(e->subevent == ORDER_EVENT_FAIL)
    {
        std::cout << e->reason ;
        if (_strnicmp(e->orderref+9, "117", 3) ==0 )
        {
            QString strmsg;
            //QString str = QString::fromLocal8Bit(e->reason);
            string t(e->reason);
            //QString str = e->reason;
            QString str = XTCodec::AfUtf8_ToQString(t);
            strmsg.sprintf("下单[%s]失败, ", e->orderref);
            strmsg.append(str);
            QMessageBox::critical(nullptr, "下单失败", strmsg, QMessageBox::Yes);
        }
    }
}

int    CSmartParkBox::helper_check()
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
    int nval = _spinVol->value();
    if(nval <=0)
    {
        return (-4);
    }

    //[4]价格检查
    const double dprice = _spinPrice->value();
    if(dprice < m_oKBDetail.lowerlimitprice || dprice > m_oKBDetail.upperlimitprice)
    {
        return (-5);
    }


    QVariant v = _cboxAccount->currentData();
    QOrderMgr* _curMgr = (QOrderMgr *)v.value<void *>();
    if(nullptr == _curMgr)
    {
        return (-6);
    }

    return 1;
}

int    CSmartParkBox::inputOrderCommon(char  direction, char  offsetflag, int volume)
{
    int iret = helper_check();
    if(iret <0 )
    {
        return iret;
    }

    QVariant v = _cboxAccount->currentData();
    QOrderMgr* _curMgr = (QOrderMgr *)v.value<void *>();

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
        oField.Volume = volume;

        oField.PriceType = XT_OPT_LimitPrice;

        double dprice = _spinPrice->value();
        oField.LimitPrice = dprice;

        oField.HedgeFlag[0] = XT_HF_Speculation;
        oField.HedgeFlag[1] = XT_HF_Speculation;
        if(_chkHedge->isChecked())
        {
            oField.HedgeFlag[0] = XT_HF_Hedge;
            oField.HedgeFlag[1] = XT_HF_Hedge;
        }
        else if(_chkArbitrage->isChecked())
        {
            oField.HedgeFlag[0] = XT_HF_Arbitrage;
            oField.HedgeFlag[1] = XT_HF_Arbitrage;
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

void   CSmartParkBox::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void   CSmartParkBox::on_inputbtn_clicked()
{
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

    int iret = inputOrderCommon(direction, offset, _spinVol->value() );
    if(iret >0)
    {
        return;
    }
    QString strmsg;
    switch(iret)
    {
    case (-1):strmsg=tr("请输入有效的代码");break;
    case (-2):strmsg=tr("无法获取基础行情");break;
    case (-3):strmsg=tr("没有合适的交易账号");break;
    case (-4):strmsg=tr("交易数量是无效的");break;
    case (-5):strmsg=tr("限价条件下，价格超出了涨跌停范围");break;
    case (-6):strmsg=tr("无法获取有效的下单通道,请确认委托模块是否开启");break;
    default:strmsg =tr("未知错误，请与开发人员联系");break;
    }

    QMessageBox::critical(nullptr, tr("下单校验错误"), strmsg, QMessageBox::Yes);

}

void   CSmartParkBox::on_bgGroup_direction_toggled(int, bool)
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

void   CSmartParkBox::on_bgGroup_offset_toggled(int, bool)
{
    inner_setBtnText();
}

void    CSmartParkBox::inner_setBtnText()
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

void    CSmartParkBox::on_cancelbtn_clicked()     //点击了 取消按钮
{

}

void    CSmartParkBox::on_parkbtn_clicked()       //点击了 条件预埋 按钮
{
    int iret = helper_check();
    if(iret <0 )
    {
        QString strmsg;
        switch(iret)
        {
        case (-1):strmsg=tr("请输入有效的代码");break;
        case (-2):strmsg=tr("无法获取基础行情");break;
        case (-3):strmsg=tr("没有合适的交易账号");break;
        case (-4):strmsg=tr("交易数量是无效的");break;
        case (-5):strmsg=tr("限价条件下，价格超出了涨跌停范围");break;
        case (-6):strmsg=tr("无法获取有效的下单通道,请确认委托模块是否开启");break;
        default:strmsg =tr("未知错误，请与开发人员联系");break;
        }

        QMessageBox::critical(nullptr, tr("字段校验错误"), strmsg, QMessageBox::Yes);
        return;
    }

    //账号
    QVariant v = _cboxAccount->currentData();
    QOrderMgr* _curMgr = (QOrderMgr *)v.value<void *>();

    //合约 //m_oExCode

    //买卖方向
    int nid = _gbtnDirection->checkedId();
    char direction = XT_D_Buy;
    if(nid ==1)
    {
        direction = XT_D_Sell;
    }

    //开平
    nid = _gbtnOffset->checkedId();
    char offset = XT_OF_Open;
    switch (nid)
    {
    case 0: offset = XT_OF_Open; break;
    case 1: offset = XT_OF_Close; break;
    case 2: offset = XT_OF_CloseToday; break;
    case 3: offset = XT_OF_Auto; break;
    }

    //套保
    char hedge = XT_HF_Speculation;
    if(_chkHedge->isChecked())
    {
        hedge = XT_HF_Hedge;
    }
    else if(_chkArbitrage->isChecked())
    {
        hedge = XT_HF_Arbitrage;
    }

    //价格
    double dprice = _spinPrice->value();

    //数量
    int volume = _spinVol->value();

    CSmartParkDlg*  pDlg = new CSmartParkDlg();
    CSmartParkDlg::tagInputData oData;
    {
        oData.curMgr = _curMgr;
        oData.ExCode = m_oExCode;
        oData.direction = direction;
        oData.offset = offset;
        oData.hedge = hedge;
        oData.price = dprice;
        oData.volume = volume;
    }
    CSmartParkDlg::tagInputDetail oDetail;
    {
        oDetail.pricetick = m_oKBDetail.pricetick;
        oDetail.showdot = m_oKBDetail.showdot;      
        oDetail.lotsize = m_oKBDetail.lotsize;       
        oDetail.multiple = m_oKBDetail.multiple;     
                
        oDetail.upperlimitprice = m_oKBDetail.upperlimitprice;
        oDetail.lowerlimitprice = m_oKBDetail.lowerlimitprice;
        oDetail.defaultprice = m_oKBDetail.defaultprice; 
        oDetail.dyn_valid = m_oKBDetail.dyn_valid;  
    }
    pDlg->setModal(true);
    pDlg->setData(&oData, &oDetail);
    pDlg->exec();

}













