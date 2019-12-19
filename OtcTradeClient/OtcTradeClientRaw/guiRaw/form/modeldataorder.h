#ifndef MODELDATA_ORDER_DEFINE_H
#define MODLEDATA_ORDER_DEFINE_H

#include "XTTradestruct.h"
#include <QtWidgets>
#include <vector>

using namespace std;

class QOrderMgr;
class ModelSingleOrder :public QAbstractTableModel
{
public:
    ModelSingleOrder(QObject *parent = nullptr);
    enum ORDER_TO_SHOW
    {
        ANY_ORDER,          //所有Order
        ACCEPTED_ORDER,     //挂单
    };

public:
    void        release();
    void        setOrderMgr(QOrderMgr* p);
    void        setOrderToShow(ORDER_TO_SHOW toShow);
    QOrderMgr*  getMgr(){return m_pMgr;}

public:
    QColor      getColorByDirection(char Direction) const;
    QColor      getColorByOffset(char offset) const;
    int         getAlignByDirection(char Direction) const;

public:
    int         rowCount(const QModelIndex &parent) const;
    int         columnCount(const QModelIndex &parent) const;

    QVariant    headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant    data(const QModelIndex &index, int role) const;

public:
    tagXTOrderField*    getDataByRow(const QModelIndex& index);

protected:
    QOrderMgr*                      m_pMgr;
    vector<tagXTOrderField>         m_arrOrderTable;
    ORDER_TO_SHOW                   m_showOrderType;
};


#endif
