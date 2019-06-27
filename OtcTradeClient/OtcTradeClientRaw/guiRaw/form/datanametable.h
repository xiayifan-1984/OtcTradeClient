#ifndef DATANAMETABLE_H
#define DATANAMETABLE_H

#include <QtWidgets>


class QExchange;
class DataNameTable : public QWidget
{
    Q_OBJECT

public:
    DataNameTable();
    ~DataNameTable();

protected:
    void                initUI();
    void                initControls();

private slots:
    void                on_combox_indexChange(int index);
    void                on_treewidget_selectChange();

private:
    void                initCodeList(int id);
    void                initCodeGrid(int id);
    void                resetCodeList();
    void                resetCodeGrid();

private:
    QExchange*          _curExchange;

    QHBoxLayout *       horizontalLayout;
    QVBoxLayout *       verticalLayout;
    QComboBox *         comboBox;
    QTreeWidget *       treeWidget;
    QTableWidget *      codetblWidget;

private:
    enum treeitemtype
    {
        itemRootFuture=1001, itemRootOption, itemFuture, itemOption
    };
    QIcon               _exchIco;
    QIcon               _productIco;

    QTabWidget*         tabWidget;
    QTableWidget*       opttblWidget;
};

#endif // DEBUGNAMETABLE_H
