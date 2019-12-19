#ifndef TWAPSHOWDETAIL_H
#define TWAPSHOWDETAIL_H

#include <QtWidgets>
#include "trademodule.h"

class ModelTwapOrder;
class CTwapShowDetail:public QWidget
{
    Q_OBJECT
public:
    explicit    CTwapShowDetail(QObject* parent = nullptr);
    void        initData(const std::string& twapRef, QOrderMgr* p);

protected:
    void        initControls();

private slots:
    void        on_tableview_doubleClicked(const QModelIndex& index);

private:
    QTableView*         _tabView;

    std::string         _twapRef;
    QOrderMgr*          _orderMgr;
    ModelTwapOrder*     _model;

};

#endif // TWAPSHOWDETAIL_H
