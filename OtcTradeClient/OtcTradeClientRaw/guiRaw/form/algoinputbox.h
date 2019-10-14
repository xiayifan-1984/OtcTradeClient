#ifndef ALGOINPUTBOX_H
#define ALGOINPUTBOX_H

#include <QtWidgets>
#include "XTBase.h"

class CQuoteCtrl;
class AlgoInputBox: public QWidget
{
    Q_OBJECT
public:
    explicit AlgoInputBox(QWidget* parent = nullptr);

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

    void            initCompleter();
    void            setCurCommodity(tagXTInstrument& oCode);
    void            initAccount(const char* pProductID);
private slots:
    void            onClickOpenChk();
    void            onClickCloseChk();
    void            onClickCloseToday();
    void            onClickAutoChk();
    void            onClickBuyChk();
    void            onClickSellChk();
    void            onCodePressReturn();
    void            onClickTwap();
protected:
    QLabel*         _lblCode;
    QLineEdit*      _editCode;
    QPushButton*    _btnSelCode;

    QCompleter*         _completer;
    QStringListModel*   _codelistModel;

    QLabel*         _lblAccount;
    QComboBox*      _cboxAccount;

    QLabel*         _lblVol;
    QSpinBox*       _spinVol;

    QLabel*         _lblOffset;
    QLabel*         _lblOpen;
    QLabel*         _lblClose;
    QLabel*         _lblAuto;
    QLabel*         _lblCloseToday;
    QCheckBox*      _chkOpen;
    QCheckBox*      _chkClose;
    QCheckBox*      _chkAuto;
    QCheckBox*      _chkCloseToday;

    QLabel*         _lblLs;
    QLabel*         _lblBuy;
    QLabel*         _lblSell;
    QCheckBox*      _chkBuy;
    QCheckBox*      _chkSell;

    QLabel*         _lblStart;
    QLabel*         _lblEnd;
    QTimeEdit*      _twapBegin;
    QTimeEdit*      _twapEnd;

    QPushButton*    _btnInsertTwap;

private:
    void            iniControl();
    int             inputTwapOrder(QTime& from, QTime& to);
    void            setStartAsCurrent();
};

#endif // ALGOINPUTBOX_H
