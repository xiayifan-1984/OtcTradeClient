#ifndef POSITIONTABLEVIEW_DEFINE_H
#define POSITIONTABLEVIEW_DEFINE_H
//持仓的补充画面
#include <QtWidgets>

//===========================================================================================================================
//合约列表
class CContractDataModel;
class CContractTableView : public QTableView
{
    Q_OBJECT
public:
    CContractTableView(QWidget *parent = nullptr);
    ~CContractTableView();

protected:
    void        initControls();

private:
    CContractDataModel*     _dataModel;

};


//===========================================================================================================================
//信息日志
class CLogTableView : public QTableView
{
    Q_OBJECT
public:
    explicit CLogTableView(QWidget *parent = nullptr);
    ~CLogTableView();

protected:
    void        initControls();
    void        initHeaders();

private:
    QStandardItemModel*     _stdModel;
    /* data */

};


//===========================================================================================================================
//持仓明细





#endif
