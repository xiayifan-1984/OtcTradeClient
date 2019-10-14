#include "condorderbox.h"
#include <QString>

static inline QString priceType2Text(PriceTypes pType)
{
    QString result;
    if(pType == PRICE_LASTEST)
    {
        result += "最新价 ";
    }
    if(pType == PRICE_BUYONE)
    {
        result += "买一价";
    }
    if(pType == PRICE_SELLONE)
    {
        result += "卖一价";
    }
    return result;
}

static inline QString dir2Text(BUY_SELL bs)
{
    QString result = "无";
    if(bs == BUY_SELL::BUY)
    {
        result = "买";
    }
    if(bs == BUY_SELL::SELL)
    {
        result = "卖";
    }
    return result;
}

static inline QString action2Text(OPEN_CLOSE oc)
{
    QString result = "无";
    if(oc == OPEN_CLOSE::OPEN)
    {
        result = "开";
    }
    if(oc == OPEN_CLOSE::CLOSE)
    {
        result = "平";
    }
    if(oc == OPEN_CLOSE::AUTO)
    {
        result = "(自动)";
    }
    if(oc == OPEN_CLOSE::CLOSE_TODAY)
    {
        result = "平今";
    }
    return result;
}
static inline QString compType2Text(CompTypes pComp)
{
    QString result;
    if(pComp == GREATER)
    {
        result += "> ";
    }
    if(pComp == NOT_LESS)
    {
        result += ">= ";
    }
    if(pComp == LESS)
    {
        result += "< ";
    }
    if(pComp == NOT_GREATER)
    {
        result += "<= ";
    }
    return result;
}

CondOrderBox::CondOrderBox(QWidget *parent)
{
    _condPrice = nullptr;
    _compType = nullptr;
    _priceType = nullptr;
    _orderInfo = nullptr;
    m_pType = PRICE_LASTEST;
    m_pComp = GREATER;
    m_condPrice = -1;
    m_insertPrice = -1;

    m_ocAction = OPEN_CLOSE::NONE;
    m_bsDirection = BUY_SELL::NONE;

    memset(&m_oKBDetail, 0, sizeof(m_oKBDetail));
    memset(&m_code, 0, sizeof(m_code));
    initControls();
    initTriggerTab();
}

void CondOrderBox::setCurCommodity(const tagXTInstrument &oCode, void* oKBDetail)
{
    if(!oKBDetail) return;
    memcpy(&m_code, &oCode, sizeof(m_code));
    memcpy(&m_oKBDetail, oKBDetail, sizeof(m_oKBDetail));
    if(!_condPrice) return;
   /* if(m_oKBDetail.dyn_valid)
    {
        _condPrice->setMinimum(m_oKBDetail.lowerlimitprice);
        _condPrice->setMaximum(m_oKBDetail.upperlimitprice);

        _condPrice->setSingleStep(m_oKBDetail.pricetick);
        _condPrice->setDecimals(m_oKBDetail.showdot);
    }
    else*/
    {
        _condPrice->setMinimum(0);
        _condPrice->setMaximum(10000000);

        _condPrice->setSingleStep(m_oKBDetail.pricetick);
        _condPrice->setDecimals(m_oKBDetail.showdot);
    }

    //[4]设置当前最新价,如果没有最新价，则设置为0
    if(m_oKBDetail.dyn_valid)
    {
        _condPrice->setValue(m_oKBDetail.defaultprice);
    }
    else
    {
        _condPrice->setValue(0);
    }
    genOrderInfo();
}

void CondOrderBox::setInitPrice(double price)
{
    m_condPrice = price;
   // m_insertPrice = price;
    _condPrice->setValue(price);
}

void CondOrderBox::setInsertPrcie(double price)
{
    m_insertPrice = price;
    genOrderInfo();
}

void CondOrderBox::reset()
{
    memset(&m_oKBDetail, 0, sizeof(m_oKBDetail));
    memset(&m_code, 0, sizeof(m_code));

    m_ocAction = OPEN_CLOSE::NONE;
    m_bsDirection = BUY_SELL::NONE;
    genOrderInfo();
}

void CondOrderBox::setBsAndOffset(BUY_SELL bs, OPEN_CLOSE oc)
{
    m_bsDirection = bs;
    m_ocAction = oc;
    genOrderInfo();
}

void CondOrderBox::setAction(OPEN_CLOSE oc)
{
    m_ocAction = oc;
    genOrderInfo();
}

void CondOrderBox::enableController()
{
    _condPrice->setDisabled(false);
    _compType->setDisabled(false);
    _priceType->setDisabled(false);
    if(m_pComp == GREATER || m_pComp == NOT_LESS)
    {
        m_bsDirection = BUY_SELL::BUY;
    }
    if(m_pComp == LESS || m_pComp == NOT_GREATER)
    {
        m_bsDirection = BUY_SELL::SELL;
    }
    genOrderInfo();
}

void CondOrderBox::disableController()
{
    _condPrice->setDisabled(true);
    _compType->setDisabled(true);
    _priceType->setDisabled(true);
}

XTContingentConditionType CondOrderBox::getContigentType()
{
    return m_triggerTab[m_pComp][m_pType];
}

double CondOrderBox::getStopPrice()
{
    return m_condPrice;
}

char CondOrderBox::getDirection()
{
    if(m_bsDirection == BUY_SELL::BUY)
    {
        return XT_D_Buy;
    }
    if(m_bsDirection == BUY_SELL::SELL)
    {
        return XT_D_Sell;
    }
    QString str;
    str.sprintf("无效的买卖方向!");
    QMessageBox::critical(nullptr, "下单失败", str, QMessageBox::Yes);
    return '\0';
}

char CondOrderBox::getOffset()
{
    if(m_ocAction == OPEN_CLOSE::OPEN)
    {
        return XT_OF_Open;
    }
    if(m_ocAction == OPEN_CLOSE::CLOSE)
    {
        return XT_OF_Close;
    }
    if(m_ocAction == OPEN_CLOSE::CLOSE_TODAY)
    {
        return XT_OF_CloseToday;
    }
    if(m_ocAction == OPEN_CLOSE::AUTO)
    {
        return XT_OF_Auto;
    }
    QString str;
    str.sprintf("无效的开平动作!");
    QMessageBox::critical(nullptr, "下单失败", str, QMessageBox::Yes);
    return XT_OF_Invalid_Offset;
}

void CondOrderBox::onSelectPriceType()
{
    m_pType = (PriceTypes)_priceType->currentIndex();
    genOrderInfo();
}

void CondOrderBox::onSelectCompType()
{
    m_pComp = (CompTypes)_compType->currentIndex();
    if(m_pComp == GREATER || m_pComp == NOT_LESS)
    {
        m_bsDirection = BUY_SELL::BUY;
    }
    if(m_pComp == LESS || m_pComp == NOT_GREATER)
    {
        m_bsDirection = BUY_SELL::SELL;
    }
    genOrderInfo();
}

void CondOrderBox::onCondPriceChanged()
{
    m_condPrice = (_condPrice->text()).toDouble();
    genOrderInfo();
}

void CondOrderBox::initControls()
{

    QGridLayout* g = new QGridLayout();
    {
        QHBoxLayout* h1 = new QHBoxLayout();

        _compType = new QComboBox();
        _compType->addItem(">", GREATER);
        _compType->addItem(">=", NOT_LESS);
        _compType->addItem("<", LESS);
        _compType->addItem("<=", NOT_GREATER);

        _priceType = new QComboBox();
        _priceType->addItem("最新价", PRICE_LASTEST);
        _priceType->addItem("买一价", PRICE_BUYONE);
        _priceType->addItem("卖一价", PRICE_SELLONE);

        _condPrice = new QDoubleSpinBox();

        h1->addWidget(_priceType, 4);
        h1->addWidget(_compType, 4);
        h1->addWidget(_condPrice,4);

        QLabel* orderInfo = new QLabel();
        QLabel* sline1 = new QLabel();
        QLabel* sline2 = new QLabel();

        orderInfo->setText("订单信息---------------");
        sline1->setText("-----------------------");
        QHBoxLayout* h2 = new QHBoxLayout();
        h2->addWidget(orderInfo,4);
        h2->addWidget(sline1,4);

        _orderInfo = new QLabel();
        _orderInfo->setText("交易品种未设置");

        QHBoxLayout* h3 = new QHBoxLayout();
        h3->addWidget(_orderInfo, 4);

        g->addLayout(h1, 0, 0);
        g->addLayout(h2, 1, 0);
        g->addLayout(h3, 2, 0);
    }
    this->setLayout(g);
    this->setWindowTitle("condition order parameters");

    connect(_priceType, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectPriceType()));
    connect(_compType, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectCompType()));
    connect(_condPrice, SIGNAL(valueChanged(const QString &)), this, SLOT(onCondPriceChanged()));
}

void CondOrderBox::genOrderInfo()
{
    QString outInfo("");
    if(m_code.ExchID <= 0)
    {
        outInfo += "交易品种未设置";
        _orderInfo->setText(outInfo);
        return;
    }

    QString instName(m_code.Code);
    instName += " ";
    outInfo += instName;

    QString pTy(priceType2Text((PriceTypes)_priceType->currentIndex()));
    outInfo += pTy;
    outInfo += " ";

    QString pComp(compType2Text((CompTypes)_compType->currentIndex()));
    outInfo += pComp;

    QString pPrice = _condPrice->text();
    outInfo += pPrice;
    outInfo += " ";

    outInfo += "触发 ";

    QString dir = dir2Text(m_bsDirection);
    QString ocAction = action2Text(m_ocAction);

    outInfo += dir;
    outInfo += ocAction;

    outInfo += " 价格";
    outInfo += QString::number(m_insertPrice, 'f', 2);
    _orderInfo->setText(outInfo);
}

void CondOrderBox::initTriggerTab()
{
    m_triggerTab[GREATER][PRICE_LASTEST] = XT_CC_LastPriceGreaterThanStopPrice;
    m_triggerTab[GREATER][PRICE_BUYONE] = XT_CC_BidPriceGreaterThanStopPrice;
    m_triggerTab[GREATER][PRICE_SELLONE] = XT_CC_AskPriceGreaterThanStopPrice;

    m_triggerTab[NOT_LESS][PRICE_LASTEST] = XT_CC_LastPriceGreaterEqualStopPrice;
    m_triggerTab[NOT_LESS][PRICE_BUYONE] = XT_CC_BidPriceGreaterEqualStopPrice;
    m_triggerTab[NOT_LESS][PRICE_SELLONE] = XT_CC_AskPriceGreaterEqualStopPrice;

    m_triggerTab[LESS][PRICE_LASTEST] = XT_CC_LastPriceLesserThanStopPrice;
    m_triggerTab[LESS][PRICE_BUYONE] =  XT_CC_BidPriceLesserThanStopPrice;
    m_triggerTab[LESS][PRICE_SELLONE] = XT_CC_AskPriceLesserThanStopPrice;

    m_triggerTab[NOT_GREATER][PRICE_LASTEST] = XT_CC_LastPriceLesserEqualStopPrice;
    m_triggerTab[NOT_GREATER][PRICE_BUYONE] =  XT_CC_BidPriceLesserEqualStopPrice;
    m_triggerTab[NOT_GREATER][PRICE_SELLONE] = XT_CC_AskPriceLesserEqualStopPrice;
}
