#ifndef MODELDATAPOSITION_DEFINE_H
#define MODELDATAPOSITION_DEFINE_H

#include "XTTradestruct.h"
#include <QtWidgets>
#include <vector>
using namespace std;

class QOrderMgr;
class ModelNormalPosition :public QAbstractTableModel
{
public:
    ModelNormalPosition(QObject *parent = nullptr);

public:
    void        release();
    void        setOrderMgr(QOrderMgr* p);

public:
    int         rowCount(const QModelIndex &parent) const;
    int         columnCount(const QModelIndex &parent) const;
    QVariant    headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant    data(const QModelIndex &index, int role) const;

protected:
    QOrderMgr*                      m_pMgr;
    vector<tagXTInvestorPositionField>  m_arrPosTable;

};


class ModelCompositePosition:public QAbstractTableModel
{
public:
    ModelCompositePosition(QObject *parent = nullptr);

public:
    void        release();
    void        setOrderMgr(QOrderMgr* p);

public:
    int         rowCount(const QModelIndex &parent) const;
    int         columnCount(const QModelIndex &parent) const;
    QVariant    headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant    data(const QModelIndex &index, int role) const;

protected:
    QOrderMgr*                      m_pMgr;
    vector<tagXTInvestorPositionField>  m_arrPosTable;

};





#endif
