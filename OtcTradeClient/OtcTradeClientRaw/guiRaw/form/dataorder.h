
#ifndef DATAORDERDEFINE_H
#define DATAORDERDEFINE_H

#include <QtWidgets>

class OrderEventArgs;
class ModelSingleOrder;
class MultiOrderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MultiOrderWidget(QWidget *parent = nullptr);
    ~MultiOrderWidget();

protected:
    void                initControls();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                on_combox_indexChange(int index);
    void                onOrderEvent(OrderEventArgs*);
    void                on_tableview_doubleClicked(const QModelIndex&);

private:
    QLabel*             _label;
    QComboBox*          _combox;

    QTableView*         _tableview;
    ModelSingleOrder*   _customModel;

    QTableView*         _tableViewAccepetedOrders;
    ModelSingleOrder*   _acceptedOrders;

    QTabWidget*         _tabWidget;

};




#endif




