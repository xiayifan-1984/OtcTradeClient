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

public:
    int         rowCount(const QModelIndex &parent) const;
    int         columnCount(const QModelIndex &parent) const;
    QVariant    headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant    data(const QModelIndex &index, int role) const;

protected:
    QOrderMgr*                      m_pMgr;
    vector<tagXTTradeField>         m_arrTradeTable;

};



class ModelTotalTrade :public QAbstractTableModel
{
public:
    ModelTotalTrade(QObject *parent = nullptr);

    void        release();

    void        setOrderMgr(QOrderMgr* p);

public:
    int         rowCount(const QModelIndex &parent) const;
    int         columnCount(const QModelIndex &parent) const;
    QVariant    headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant    data(const QModelIndex &index, int role) const;
    //virtual void  sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

protected:
    QOrderMgr*                      m_pMgr;

    struct tagTradeSumInfo
    {
        tagXTInstrument             ExCode;         //合约  交易所
        char                        Product[12];    //品种
        char                        Direction;      //方向
        char                        Offset;         //开平
        double                      Average;        //均价
        int                         Volume;         //手数
        char                        Hedge;          //套保
    };

    vector<tagTradeSumInfo>         m_arrTradeSum;

};




#endif
