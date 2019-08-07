#ifndef CONDORDERBOX_H
#define CONDORDERBOX_H

#include <QtWidgets>
#include "XTBase.h"

typedef enum
{
    PRICE_LASTEST = 0,
    PRICE_BUYONE,
    PRICE_SELLONE
}PriceTypes;

typedef enum
{
    GREATER = 0,
    NOT_LESS,
    LESS,
    NOT_GREATER,
}CompTypes;

enum class BUY_SELL
{
    NONE,
    BUY,
    SELL
};

enum class OPEN_CLOSE
{
    NONE,
    OPEN,
    CLOSE,
    AUTO
};

class CondOrderBox: public QWidget
{
    Q_OBJECT
public:
    explicit CondOrderBox(QWidget* parent = nullptr);
    ~CondOrderBox(){}

private:
    struct tagKBDetail
    {
        double          pricetick;          //价格Tick
        int             showdot;            //显示小数位数
        int             lotsize;            //手比率
        int             multiple;           //合约乘数

        double          upperlimitprice;    //涨停价
        double          lowerlimitprice;    //跌停价
        double          defaultprice;       //当前建议值
        bool            dyn_valid;              //upper,lower, default 三个值是否有效
    }m_oKBDetail;

public:
    void setCurCommodity(const tagXTInstrument& oCode, void* oKBDetail);
    void setInitPrice(double price);
    void reset();
    void setBsAndOffset(BUY_SELL bs, OPEN_CLOSE oc);

private slots:
    void onSelectPriceType();
    void onSelectCompType();
    void onCondPriceChanged();

private:

    void initControls();
    void genOrderInfo();
    QComboBox*  _priceType;
    QComboBox*  _compType;
    QDoubleSpinBox* _condPrice;

    QLabel* _orderInfo;

    tagXTCommodity m_code;
    PriceTypes m_pType;
    CompTypes m_pComp;
    double m_condPrice;

    BUY_SELL m_bsDirection;
    OPEN_CLOSE m_ocAction;
};

#endif // CONDORDERBOX_H
