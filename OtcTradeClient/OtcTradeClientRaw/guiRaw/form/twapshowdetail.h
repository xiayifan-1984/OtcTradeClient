#ifndef TWAPSHOWDETAIL_H
#define TWAPSHOWDETAIL_H

#include <QtWidgets>
#include "modeldataorder.h"
#include "ordermgrimpl.h"

class TwapShowDetail:public QWidget
{
    Q_OBJECT
public:
    explicit TwapShowDetail(QObject* parent = nullptr);
    void setContent(QTableView* pView, ModelSingleOrder* pModel,QOrderMgr* user);

private slots:
    void  on_tableview_doubleClicked(const QModelIndex& index);
private:
    QTableView* _tabView;
    QOrderMgr* _user;
    ModelSingleOrder* _model;
};

#endif // TWAPSHOWDETAIL_H
