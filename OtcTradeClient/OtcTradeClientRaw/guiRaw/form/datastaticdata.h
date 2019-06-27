#ifndef DATASTATICDATA_H
#define DATASTATICDATA_H

#include <QtWidgets>


class QExchange;
class StaticDataModel;
class DataStaticData : public QWidget
{
    Q_OBJECT

public:
    DataStaticData();
    ~DataStaticData();

protected:
    void                initUI();
    void                initControls();

private slots:
    void                on_combox_indexChange(int index);
    void                on_treewidget_selectChange();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    QExchange*          _curExchange;

    QHBoxLayout *       horizontalLayout;
    QVBoxLayout *       verticalLayout;
    QComboBox *         comboBox;
    QTreeWidget *       treeWidget;
    QTableView*         codetblWidget;

private:
    enum treeitemtype
    {
        itemRootFuture=1001, itemRootOption, itemFuture, itemOption
    };
    QIcon               _exchIco;
    QIcon               _productIco;

    StaticDataModel*    _customModel;
};

#endif // DEBUGNAMETABLE_H
