#ifndef SELECTCOMMODITYDEFINE_H
#define SELECTCOMMODITYDEFINE_H

#include <QtWidgets>
#include "XTBase.h"

#ifndef GLOBAL_ONE_DEFINE_TAGXTINSTRUMENT
#define GLOBAL_ONE_DEFINE_TAGXTINSTRUMENT
Q_DECLARE_METATYPE(tagXTInstrument)
#endif


//选择商品对话框
class QExchange;
class UpDownLineEdit;
class SelectCommodity : public QDialog
{
    Q_OBJECT

public:
    explicit SelectCommodity(QWidget* parent = nullptr);
    ~SelectCommodity();

public:
    void                getSelectCommodity(tagXTInstrument* pIn);

protected:
    void                initControls();

private slots:
    void                onOK();
    void                onCancel();
    void                onCodetextChanged(const QString &text);
    void                on_combox_indexChange(int index);
    void                on_listwidget_selectChange();
    void                onlistcodeDoubleCliecked(QListWidgetItem*);
    void                oneditkeyup();
    void                oneditkeydown();

protected:
    QLabel*             _lblContract;
    QLabel*             _lblExchange;
    UpDownLineEdit*     _editContract;
    QComboBox*          _cboxExchange;
    QListWidget*        _listProduct;
    QListWidget*        _listCode;

    QPushButton*        _btnOK;
    QPushButton*        _btnCancel;

protected:
    QExchange*          _curExchange;

    tagXTInstrument     _curselCode;

};








#endif
