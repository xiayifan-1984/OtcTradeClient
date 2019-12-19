#ifndef SMARTPARKBOX_DEFINE_H
#define SMARTPARKBOX_DEFINE_H

//SmartParkBox 和SmartInputBox 基本一致，仅增加了2个按钮
#include <QtWidgets>
#include "XTBase.h"
#include <utility>

class CQuoteCtrl;
class OrderEventArgs;
class CSmartParkBox: public QWidget
{
    Q_OBJECT

public:
    explicit CSmartParkBox(QWidget* parent = nullptr);
    ~CSmartParkBox();

public:

protected:
    void            initCompleter();
    void            initControls();     //初始化控件
    void            configControls();   //控件默认状态

protected:
    void            setCurCommodity(tagXTInstrument& oCode);
    void            initAccount(const char* pProductID);
    int             inputOrderCommon(char  direction, char  offsetflag, int volume);
    int             helper_check();

private slots:
    void            onQuoteClick(int idx);  //当点击行情板上的价格时
    void            onCodePressReturn();    //当输入代码，回车时
    void            onOrderEvent(OrderEventArgs*);
    void            on_bgGroup_direction_toggled(int, bool);
    void            on_bgGroup_offset_toggled(int, bool);

    void            on_inputbtn_clicked();      //点击了下单按钮
    void            on_cancelbtn_clicked();     //点击了 取消按钮
    void            on_parkbtn_clicked();       //点击了 条件预埋 按钮

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
    CQuoteCtrl*         _drawQuote;

    //[line1]
    QLabel*             _lblCode;
    QLineEdit*          _editCode;

    QCheckBox*          _chkArbitrage;      //套利
    QCheckBox*          _chkHedge;          //套保

    //[line2]
    QLabel*             _lblAccount;
    QComboBox*          _cboxAccount;

    //[line3]
    QLabel*             _lblDirection;
    QRadioButton*       _rbtnBuy;
    QRadioButton*       _rbtnSell;
    QButtonGroup*       _gbtnDirection;

    //[line4]
    QLabel*             _lblOffset;
    QRadioButton*       _rbtnOpen;
    QRadioButton*       _rbtnClose;
    QRadioButton*       _rbtnCloseToday;
    QRadioButton*       _rbtnAuto;
    QButtonGroup*       _gbtnOffset;

    //[line5]
    QLabel*             _lblVol;
    QSpinBox*           _spinVol;

    //[line6]
    QLabel*             _lblPrice;
    QDoubleSpinBox*      _spinPrice;

    //[line7]
    QPushButton*        _btnInput;      //下单按钮
    QPushButton*        _btnCancel;     //撤单按钮
    QPushButton*        _btnPark;       //条件预埋 按钮

protected:
    QCompleter*         _completer;
    QStringListModel*   _codelistModel;

private:
    void                closeEvent(QCloseEvent *event);
    void                inner_setBtnText();
};







#endif

