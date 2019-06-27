#ifndef MODELDATATRADE_DEFINE_H
#define MODELDATATRADE_DEFINE_H

#include "XTTradestruct.h"
#include <QtWidgets>
#include <vector>
using namespace std;

class QOrderMgr;
class ModelSingleTrade :public QAbstractTableModel
{
public:
    ModelSingleTrade(QObject *parent = nullptr);

    void        release();

    void        setOrderMgr(QOrderMgr* p);

    QColor      getColorByDirection(char Direction) const;

    QColor      getColorByOffset(char offset) const;

    int         getAlignByDirection(char Direction) const;

public:
    int         rowCount(const QModelIndex &parent) const;
    int         columnCount(const QModelIndex &parent) const;
    QVariant    headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant    data(const QModelIndex &index, int role) const;

protected:
    QOrderMgr*                      m_pMgr;
    vector<tagXTTradeField>         m_arrTradeTable;

};







#endif
