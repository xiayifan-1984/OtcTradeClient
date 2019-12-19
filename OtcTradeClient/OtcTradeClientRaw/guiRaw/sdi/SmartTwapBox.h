
#ifndef SMARTTWAPBOX_DEFINE_H
#define SMARTTWAPBOX_DEFINE_H

#include <QtWidgets>
#include "XTBase.h"
#include <utility>


class CSmartTwapBox: public QWidget
{
    Q_OBJECT

public:
    explicit CSmartTwapBox(QWidget* parent = nullptr);
    ~CSmartTwapBox();

protected:
    void            initCompleter();
    void            initControls(); //初始化控件
    void            configControls();  //控件默认状态

protected:
    void            setCurCommodity(tagXTInstrument& oCode);
    void            initAccount(const char* pProductID);

private slots:
    void            onCodePressReturn();    //当输入代码，回车时
    void            on_inputbtn_clicked();      //点击了下单按钮
    void            on_bgGroup_direction_toggled(int, bool);
    void            on_bgGroup_offset_toggled(int, bool);

protected:
    tagXTInstrument     m_oExCode;

protected:
    //[line1]
    QLabel*         _lblCode;
    QLineEdit*      _editCode;

    //[line2]
    QLabel*         _lblAccount;
    QComboBox*      _cboxAccount;

    //[line3]
    QLabel*         _lblDirection;
    QRadioButton*   _rbtnBuy;
    QRadioButton*   _rbtnSell;
    QButtonGroup*   _gbtnDirection;

    //[line4]
    QLabel*         _lblOffset;
    QRadioButton*   _rbtnOpen;
    QRadioButton*   _rbtnClose;
    QRadioButton*   _rbtnCloseToday;
    QRadioButton*   _rbtnAuto;
    QButtonGroup*   _gbtnOffset;

    //[line5]
    QLabel*         _lblVol;
    QSpinBox*       _spinVol;

    //[line6]开始时间  结束时间
    QLabel*         _lblStart;
    QLabel*         _lblEnd;
    QTimeEdit*      _twapBegin;
    QTimeEdit*      _twapEnd;

    //[line7]
    QPushButton*    _btnInput;

protected:
    QCompleter*         _completer;
    QStringListModel*   _codelistModel;

private:
    void                closeEvent(QCloseEvent *event);
    void                inner_setBtnText();
    int                 inputTwapOrder(QTime& from, QTime& to);

};


#endif
