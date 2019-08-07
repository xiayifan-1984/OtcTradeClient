#ifndef PARKEDORDERTAB_H
#define PARKEDORDERTAB_H

#include <QtWidgets>

class ParkedOrderTab : public QTableView
{
    Q_OBJECT
public:
    explicit ParkedOrderTab(QWidget* parent = nullptr);
    ~ParkedOrderTab(){}

private:
    void initControls();
    void setHeadersForParkedOrder();
    QStandardItemModel* _parkedOrderModel;
};

#endif // PARKEDCONDORDERTAB_H
