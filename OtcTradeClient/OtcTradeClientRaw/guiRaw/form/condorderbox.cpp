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
    QString result;
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
    QString result;
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

    m_ocAction = OPEN_CLOSE::NONE;
    m_bsDirection = BUY_SELL::NONE;

    memset(&m_oKBDetail, 0, sizeof(m_oKBDetail));
    memset(&m_code, 0, sizeof(m_code));
    initControls();
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
    _condPrice->setValue(price);
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

void CondOrderBox::onSelectPriceType()
{
    genOrderInfo();
}

void CondOrderBox::onSelectCompType()
{
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

    QString pComp(compType2Text((CompTypes)_compType->currentIndex()));
    outInfo += pComp;

    outInfo += " ";

    QString pPrice = _condPrice->text();
    outInfo += pPrice;
    outInfo += " ";

    outInfo += "触发 ";

    QString dir = dir2Text(m_bsDirection);
    QString ocAction = action2Text(m_ocAction);

    outInfo += dir;
    outInfo += ocAction;
    _orderInfo->setText(outInfo);
}
