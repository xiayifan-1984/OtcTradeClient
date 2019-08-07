#ifndef CONDORDERTAB_H
#define CONDORDERTAB_H
#include <QtWidgets>

class CondOrderTab: public QTableView
{
public:
    explicit CondOrderTab(QWidget* parent = nullptr);
    ~CondOrderTab(){}

private:
    void initControls();
    void setHeadersForCondOrder();
    QStandardItemModel* _condOrderModel;
    void fillRow(int rowIdx);
};

#endif // CONDORDERTAB_H
