#ifndef PARKEDORDERTAB_H
#define PARKEDORDERTAB_H

#include <QtWidgets>
#include <memory>
#include <parkordermgr.h>
#include <unordered_map>
#include <string>

class ParkedOrderTab : public QTableView
{
    Q_OBJECT
public:
    explicit ParkedOrderTab(QWidget* parent = nullptr);
    ~ParkedOrderTab(){}

    void setUserMgr(std::shared_ptr<ParkCondOrderMgrByUser> pUserMgr);
    std::string findParkId(int rowIdx);

private:
    void initControls();
    void setHeadersForParkedOrder();
    QStandardItemModel* _parkedOrderModel;
    void fillRow(int row, tagXTParkedOrderField& parkOrder);
    std::unordered_map<int, std::string> m_rowIdx2ParkId;
};

#endif // PARKEDCONDORDERTAB_H
