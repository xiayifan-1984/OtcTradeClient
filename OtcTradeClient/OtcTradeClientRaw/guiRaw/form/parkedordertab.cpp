#include "parkedordertab.h"
#include "stool.h"
#include <algorithm>

ParkedOrderTab::ParkedOrderTab(QWidget *parent)
{
    _parkedOrderModel = nullptr;
    initControls();
}

void ParkedOrderTab::setUserMgr(std::shared_ptr<ParkCondOrderMgrByUser> pUserMgr)
{
    //_parkedOrderModel->removeRows(0, _parkedOrderModel->rowCount());
    if(!pUserMgr) return;
    auto parkOrders = pUserMgr->getParkOrders();
    _parkedOrderModel->clear();
    setHeadersForParkedOrder();
    m_rowIdx2ParkId.clear();
    sort(parkOrders.begin(), parkOrders.end(), [](const tagXTParkedOrderField& p1, const tagXTParkedOrderField& p2)
    {
        if(strcmp(p1.ParkedOrderID, p2.ParkedOrderID)>0)
        {
            return true;
        }
        return false;
    });
    int rowIdx = 0;
    for(auto& row:parkOrders)
    {
        string parkId(row.ParkedOrderID);
        m_rowIdx2ParkId.insert({rowIdx, parkId});
        fillRow(rowIdx, row);
        ++rowIdx;
    }
    this->repaint();
}

string ParkedOrderTab::findParkId(int rowIdx)
{
    string result;
    auto search = m_rowIdx2ParkId.find(rowIdx);
    if(search == m_rowIdx2ParkId.end())
    {
        return result;
    }
    return search->second;
}

void ParkedOrderTab::initControls()
{
    _parkedOrderModel = new QStandardItemModel();
    this->setModel(_parkedOrderModel);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHeadersForParkedOrder();
}

void ParkedOrderTab::setHeadersForParkedOrder()
{
    QStandardItem* pItem = new QStandardItem();
    pItem->setText("ID Number");
    _parkedOrderModel->setHorizontalHeaderItem(0, pItem);

    pItem = new QStandardItem();
    pItem->setText("Code");
    _parkedOrderModel->setHorizontalHeaderItem(1, pItem);

    pItem = new QStandardItem();
    pItem->setText("Status");
    _parkedOrderModel->setHorizontalHeaderItem(2, pItem);

    pItem = new QStandardItem();
    pItem->setText("Offset");
    _parkedOrderModel->setHorizontalHeaderItem(3, pItem);

    pItem = new QStandardItem();
    pItem->setText("BuyOrSell");
    _parkedOrderModel->setHorizontalHeaderItem(4, pItem);

    pItem = new QStandardItem();
    pItem->setText("Price");
    _parkedOrderModel->setHorizontalHeaderItem(5, pItem);


    pItem = new QStandardItem();
    pItem->setText("Volumn");
    _parkedOrderModel->setHorizontalHeaderItem(6, pItem);

    pItem = new QStandardItem();
    pItem->setText("OrderRef");
    _parkedOrderModel->setHorizontalHeaderItem(7, pItem);

    pItem = new QStandardItem();
    pItem->setText("InsertTime");
    _parkedOrderModel->setHorizontalHeaderItem(8, pItem);

}

void ParkedOrderTab::fillRow(int row, tagXTParkedOrderField &parkOrder)
{
    QStandardItem* pItem = new QStandardItem();
    QString strID = parkOrder.ParkedOrderID;
    pItem->setText(strID);
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);

    _parkedOrderModel->setItem(row, 0, pItem);

    pItem = new QStandardItem();
    QString strCode = parkOrder.ExCode.Code;
    pItem->setText(strCode);
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkedOrderModel->setItem(row, 1, pItem);

    pItem = new QStandardItem();
    pItem->setText(parkOrderStatusToStr(parkOrder.ParkedStatus));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    if(parkOrderStatusToStr(parkOrder.ParkedStatus) == "未报")
    {
       // pItem->setTextColor(QColor(236,41,103));
        pItem->setForeground(QBrush(QColor(236,41,103)));
    }
    _parkedOrderModel->setItem(row, 2, pItem);

    pItem = new QStandardItem();
    pItem->setText(stool::openClose2Text(parkOrder.OffsetFlag));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkedOrderModel->setItem(row, 3, pItem);

    pItem = new QStandardItem();
    pItem->setText(stool::buySell2Text(parkOrder.Direction));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkedOrderModel->setItem(row, 4, pItem);

    pItem = new QStandardItem();
    pItem->setText(QString::number(parkOrder.LimitPrice, 'f',2));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkedOrderModel->setItem(row, 5, pItem);

    pItem = new QStandardItem();
    pItem->setText(QString::number(parkOrder.Volume, 'f',2));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkedOrderModel->setItem(row, 6, pItem);

   /* pItem = new QStandardItem();
    pItem->setText("ParkTime");
    _parkedOrderModel->setItem(row, 7, pItem);*/

    pItem = new QStandardItem();
    pItem->setText(parkOrder.OrderRef);
    _parkedOrderModel->setItem(row, 7, pItem);

    /*
    pItem = new QStandardItem();
    QString str ="";
    int ntime = parkOrder.;
    str.sprintf("%02d:%02d:%02d", ntime / 10000, (ntime % 10000) / 100, ntime % 100);
    pItem->setText("InsertTime");
    _parkedOrderModel->setItem(row, 7, pItem);*/
}
