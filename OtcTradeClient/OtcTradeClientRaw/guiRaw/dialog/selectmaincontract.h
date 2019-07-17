#ifndef SELECTMAINCONTRACT_DEFINE_H
#define SELECTMAINCONTRACT_DEFINE_H

#include <QtWidgets>
#include "XTBase.h"

#ifndef GLOBAL_ONE_DEFINE_TAGXTINSTRUMENT
#define GLOBAL_ONE_DEFINE_TAGXTINSTRUMENT
Q_DECLARE_METATYPE(tagXTInstrument)
#endif

//选择主力合约对话框
class SelectMainContract : public QDialog
{
    Q_OBJECT

public:
    explicit SelectMainContract(QWidget* parent = nullptr);
    ~SelectMainContract();

public:
    void                getSelectCommodity(tagXTInstrument* pIn);

protected:
    void                initControls();
    void                initListCode();

private slots:
    void                onOK();
    void                onCancel();
    void                onlistcodeDoubleCliecked(QListWidgetItem* p);

protected:
    QLabel*             _lblTip;
    QListWidget*        _listCode;

    QPushButton*        _btnOK;
    QPushButton*        _btnCancel;

protected:
    tagXTInstrument     _curselCode;
    bool                m_isAllProduct;
};








#endif
