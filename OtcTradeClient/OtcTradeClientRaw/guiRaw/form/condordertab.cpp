#include "condordertab.h"
#include "stool.h"

CondOrderTab::CondOrderTab(QWidget *parent)
{
    _condOrderModel = nullptr;
    initControls();
}

void CondOrderTab::setUserMgr(std::shared_ptr<ParkCondOrderMgrByUser> pUserMgr)
{
    //_parkedOrderModel->removeRows(0, _parkedOrderModel->rowCount());
    if(!pUserMgr) return;
    m_rowIdx2ParkId.clear();
    auto condOrders = pUserMgr->getCondOrders();
    if(condOrders.size()<=0)return;
    const auto& parkTimes = pUserMgr->getParkTime();
    sort(condOrders.begin(), condOrders.end(), [](const tagXTParkedOrderField& p1, const tagXTParkedOrderField& p2)
    {
        if(strcasecmp(p1.ExCode.Code, p2.ExCode.Code)<0)
        {
            return true;
        }
        return false;
    });
    int rowIdx = 0;
    for(auto& row:condOrders)
    {
        string parkId(row.ParkedOrderID);
        m_rowIdx2ParkId.insert({rowIdx, parkId});
        QString ptime = "";
        auto search = parkTimes.find(parkId);
        if(search != parkTimes.end())
        {
            ptime = (search->second).toString();
        }
        fillRow(rowIdx, row, ptime);
        ++rowIdx;
    }
}

void CondOrderTab::initControls()
{
    _condOrderModel = new QStandardItemModel();
    setHeadersForCondOrder();
    this->setModel(_condOrderModel);
}

void CondOrderTab::setHeadersForCondOrder()
{
    QStandardItem* pItem = new QStandardItem();
    pItem->setText("ID Number");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(0, pItem);

    pItem = new QStandardItem();
    pItem->setText("Code");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(1, pItem);

    pItem = new QStandardItem();
    pItem->setText("Status");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(2, pItem);

    pItem = new QStandardItem();
    pItem->setText("PriceType");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(3, pItem);

    pItem = new QStandardItem();
    pItem->setText("trigger");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(4, pItem);

    pItem = new QStandardItem();
    pItem->setText("TriggerPrice");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(5, pItem);

    pItem = new QStandardItem();
    pItem->setText("Direction");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(6, pItem);

    pItem = new QStandardItem();
    pItem->setText("OpenOrClose");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(7, pItem);

    pItem = new QStandardItem();
    pItem->setText("Volumn");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(8, pItem);

    pItem = new QStandardItem();
    pItem->setText("OrderRef");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(9, pItem);

    pItem = new QStandardItem();
    pItem->setText("InsertPrice");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(10, pItem);

    pItem = new QStandardItem();
    pItem->setText("ParkTime");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(11, pItem);
}

void CondOrderTab::fillRow(int rowIdx, tagXTParkedOrderField& parkOrder, QString parkTime)
{
    int col = 0;

    //ID Number
    QStandardItem* pItem = new QStandardItem();
    pItem->setText(parkOrder.ParkedOrderID);
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col, pItem);

    //Code
    pItem = new QStandardItem();
    pItem->setText(parkOrder.ExCode.Code);
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+1, pItem);

    //Status
    pItem = new QStandardItem();
    pItem->setText(parkOrderStatusToStr(parkOrder.ParkedStatus));
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    if(parkOrder.ParkedStatus == XT_PAOS_NotSend)
    {
       // pItem->setTextColor(QColor(236,41,103));
        pItem->setForeground(QBrush(QColor(236,41,103)));
    }
    _condOrderModel->setItem(rowIdx, col+2, pItem);

    //triggerPriceType
    pItem = new QStandardItem();
    QString tp = "其他";
    if(parkOrder.ParkedType >= XT_CC_LastPriceGreaterThanStopPrice && parkOrder.ParkedType <= XT_CC_LastPriceLesserEqualStopPrice)
    {
        tp = "最新价";
    }
    if(parkOrder.ParkedType >= XT_CC_AskPriceGreaterThanStopPrice && parkOrder.ParkedType <= XT_CC_AskPriceLesserEqualStopPrice)
    {
        tp = "卖一价";
    }
    if(parkOrder.ParkedType >= XT_CC_BidPriceGreaterThanStopPrice && parkOrder.ParkedType <= XT_CC_BidPriceLesserEqualStopPrice)
    {
        tp = "买一价";
    }
    pItem->setText(tp);
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
   // if(parkOrder.ParkedStatus == XT_PAOS_NotSend)
   // {
       // pItem->setTextColor(QColor(236,41,103));
    //    pItem->setForeground(QBrush(QColor(236,41,103)));
   // }
    _condOrderModel->setItem(rowIdx, col+3, pItem);

    //trigger
    pItem = new QStandardItem();
    pItem->setText(condTriggerToStr(parkOrder.ParkedType));
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+4, pItem);

    //trigger price
    pItem = new QStandardItem();
    pItem->setText(QString::number(parkOrder.StopPrice, 'f',2));
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+5, pItem);

    //Direction
    pItem = new QStandardItem();
    pItem->setText(stool::buySell2Text(parkOrder.Direction));
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+6, pItem);

    //OpenOrClose
    pItem = new QStandardItem();
    pItem->setText(stool::openClose2Text(parkOrder.OffsetFlag));
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+7, pItem);

    //Volumn
    pItem = new QStandardItem();
    pItem->setText(QString::number(parkOrder.Volume, 'f',2));
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+8, pItem);

    //OrderRef
    pItem = new QStandardItem();
    pItem->setText(parkOrder.OrderRef);
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+9, pItem);

    //InsertPrice
    pItem = new QStandardItem();
    pItem->setText(QString::number(parkOrder.LimitPrice, 'f', 2));
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+10, pItem);

    //InsertPrice
    pItem = new QStandardItem();
    pItem->setText(parkTime);
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+11, pItem);
}

string CondOrderTab::findParkId(int rowIdx)
{
    string result;
    auto search = m_rowIdx2ParkId.find(rowIdx);
    if(search == m_rowIdx2ParkId.end())
    {
        return result;
    }
    return search->second;
}
