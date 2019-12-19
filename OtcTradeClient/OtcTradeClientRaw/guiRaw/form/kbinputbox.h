#ifndef INPUTBOXDEFINE_H
#define INPUTBOXDEFINE_H

#include <QtWidgets>
#include "XTBase.h"
#include <utility>

class CQuoteCtrl;
class OrderEventArgs;
class KBInputBox: public QWidget
{
    Q_OBJECT

public:
    explicit KBInputBox(QWidget* parent = nullptr);
    ~KBInputBox();

public:
    //void            setInputParam(tagXTInstrument* p);       //外面调用，可能还会增加参数
    void            inputCommodity(tagXTInstrument& oCode);

protected:
    void            initCompleter();
    void            initControls(); //初始化控件
    void            configControls();  //控件默认状态

protected:
    void            setCurCommodity(tagXTInstrument& oCode);
    void            initAccount(const char* pProductID);
    int             inputOrderCommon(char  direction, char  offsetflag, int volume);

private slots:
    void            onCodeBtnPushed();      //当点击按钮,选择商品时
    void            onQuoteClick(int idx);  //当点击行情板上的价格时
    void            onCodePressReturn();    //当输入代码，回车时
    void            on_buyopenbtn_clicked_or_autolong();    //买入开仓
    void            on_buyclosebtn_clicked();   //买入平仓
    void            on_sellopenbtn_clicked_or_autoshort();   //卖出开仓
    void            on_sellclosebtn_clicked();  //卖出平仓
    void            onOrderEvent(OrderEventArgs*);
   
protected:
    tagXTInstrument     m_oExCode;

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

protected:
    CQuoteCtrl*     _drawQuote;

    QLabel*         _lblCode;
    QLineEdit*      _editCode;
    QPushButton*    _btnSelCode;

    QLabel*         _lblAccount;
    QComboBox*      _cboxAccount;

    QCheckBox*      _chkHedge;  //套保

    QLabel*         _lblVol;
    QLabel*         _lblPrice;
    QSpinBox*       _spinVol;
    QDoubleSpinBox*       _spinPrice;

    QCheckBox*      _chkAny;
    QCheckBox*      _chCloseTodayOrYes;

    
    QPushButton*    _btnBuyOpen;
    QPushButton*    _btnBuyClose;
    QPushButton*    _btnSellOpen;
    QPushButton*    _btnSellClose;

protected:
    QCompleter*         _completer;
    QStringListModel*   _codelistModel;

private:
    void                 closeEvent(QCloseEvent *event);
    void                 closePostion(char direction, int vol);
};





#endif

