#ifndef TWAPINPUTBOX_H
#define TWAPINPUTBOX_H

#include <QtWidgets>
#include "XTBase.h"

class CQuoteCtrl;
class CTwapInputBox: public QWidget
{
    Q_OBJECT
public:
    explicit CTwapInputBox(QWidget* parent = nullptr);

protected:
    tagXTInstrument     m_oExCode;

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

    QCompleter*         _completer;
    QStringListModel*   _codelistModel;

    QLabel*         _lblAccount;
    QComboBox*      _cboxAccount;

    QLabel*         _lblVol;
    QSpinBox*       _spinVol;

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

#endif // TWAPINPUTBOX_H
