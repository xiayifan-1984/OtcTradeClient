#ifndef MODELDATAORDER_DEFINE_H
#define MODLEDATAORDER_DEFINE_H

#include "XTTradestruct.h"
#include <QtWidgets>
#include <vector>
#include "parkorderbox.h"
using namespace std;

enum class ORDER_TO_SHOW
{
    ANY_ORDER,
    ACCEPTED_ORDER,
    PARK_ORDER,
    INVID_ORDER
};

class QOrderMgr;
class ModelSingleOrder :public QAbstractTableModel
{
public:
    ModelSingleOrder(QObject *parent = nullptr);

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
