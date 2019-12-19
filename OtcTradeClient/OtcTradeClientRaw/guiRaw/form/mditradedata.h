#ifndef MDITRADEDATADEFINE_H
#define MDITRADEDATADEFINE_H

#include <QtWidgets>

class TransactionEventArgs;
class ModelSingleTrade;
class CMDITradeData : public QWidget
{
    Q_OBJECT

public:
    explicit CMDITradeData(QWidget *parent = nullptr);
    ~CMDITradeData();

protected:
    void                initControls();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                on_combox_indexChange(int index);
    void                onTransactEvent(TransactionEventArgs* e);

private:
    QLabel*             _label;
    QComboBox*          _combox;
    QTableView*         _tableview;
    ModelSingleTrade*   _customModel;

};




#endif
