#ifndef SMARTPARKDLG_DEFINE_H
#define SMARTPARKDLG_DEFINE_H

#include <QtWidgets>
#include "XTBase.h"

class QOrderMgr;
class CSmartParkDlg: public QDialog
{
    Q_OBJECT

public:
    explicit CSmartParkDlg(QWidget* parent = nullptr);
    ~CSmartParkDlg();

public:
    struct tagInputDetail
    {
        double          pricetick;          //价格Tick
        int             showdot;            //显示小数位数
        int             lotsize;            //手比率
        int             multiple;           //合约乘数

        double          upperlimitprice;    //涨停价
        double          lowerlimitprice;    //跌停价
        double          defaultprice;       //当前建议值
        bool            dyn_valid;              //upper,lower, default 三个值是否有效
    };
    struct tagInputData
    {
        QOrderMgr*          curMgr;
        tagXTInstrument     ExCode;
        char                direction;
        char                offset;
        char                hedge;
        double              price;
        int                 volume;
    };

    void            setData(tagInputData* pData, tagInputDetail* pDetail);

protected:
    void            initControls();     //初始化控件
    void            configControls();   //控件的默认状态
    void            reconfigControls(); //再次设置控件

private slots:
    void            on_okbtn_clicked();      //点击了OK按钮
    void            on_cancelbtn_clicked();  //点击了Cancel按钮

protected:
    //第一区域
    QLabel*         _lblPlace;
    QComboBox*      _cboxPlace;

    //第二区域
        //第一行
    QLabel*         _lblInfo;
        //第二行
    QLabel*         _lblPrice;
    QDoubleSpinBox* _spinPrice;
    QLabel*         _lblVolume;
    QSpinBox*       _spinVolume;
        //Group
    QGroupBox*      _gboxInfo;

    //第三区域
    QRadioButton*   _rbtnCondition;
    QRadioButton*   _rbtnExchContinue;
    QRadioButton*   _rbtnExchAuction;
    QRadioButton*   _rbtnTime;
    QRadioButton*   _rbtnHuman;

        //第一行
    QComboBox*      _cboxCondition;
    QComboBox*      _cboxOperator;
    QDoubleSpinBox* _spinConditionPrice;
        //第二行
    QLabel*         _lblExchContinue;
        //第三行
    QLabel*         _lblExchAuction;
        //第四行
    QLabel*         _lblTime;
    QTimeEdit*      _tedtTime;
        //第五行
    QLabel*         _lblHuman;
        //Group
    QGroupBox*      _gboxCondition;
    QButtonGroup*   _gbtnSelect;

    //第四区域
    QPushButton*    _btnOK;
    QPushButton*    _btnCancel;

protected:
   tagInputData     m_oInputData;
   tagInputDetail   m_oInputDetail;

};


















#endif
