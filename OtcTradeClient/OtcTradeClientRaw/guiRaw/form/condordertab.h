#ifndef CONDORDERTAB_H
#define CONDORDERTAB_H
#include <QtWidgets>
#include <memory>
#include <parkordermgr.h>

class CondOrderTab: public QTableView
{
public:
    explicit CondOrderTab(QWidget* parent = nullptr);
    ~CondOrderTab(){}
    void setUserMgr(std::shared_ptr<ParkCondOrderMgrByUser> pUserMgr);
    std::string findParkId(int rowIdx);

private:
    void initControls();
    void setHeadersForCondOrder();
    QStandardItemModel* _condOrderModel;
    void fillRow(int row, tagXTParkedOrderField& parkOrder, QString parkTime);


    std::unordered_map<int, std::string> m_rowIdx2ParkId;
};

#endif // CONDORDERTAB_H
