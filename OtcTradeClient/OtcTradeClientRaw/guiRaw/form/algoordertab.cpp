#include "algoordertab.h"
#include "stool.h"
#include "twapshowdetail.h"

inline QString algoStatus2Text(char status)
{
    QString result;
    if(status == XT_ALGO_Accepted)
    {
        result = "已报";
    }
    if(status == XT_AlGO_End)
    {
        result = "执行结束";
    }
    if(status == XT_AlGO_Cancelled)
    {
        result = "已撤";
    }
    if(status == XT_AlGO_Executing)
    {
        result = "执行中";
    }
    return result;
}

AlgoOrderTab::AlgoOrderTab(QWidget *parent)
{
    _algoOrderModel = nullptr;
    initControls();
}

void AlgoOrderTab::setUserMgr(AlgoOrderMgrByUser &algoUserMgr)
{
    m_rowIdx2AlgoId.clear();
    auto algoOrders = algoUserMgr.getAlgoOrders();
    if(algoOrders.size()<=0)
    {
        _algoOrderModel->removeRows(0, _algoOrderModel->rowCount());
    }
    sort(algoOrders.begin(), algoOrders.end(), [](const auto& p1, const auto& p2)
    {
        if(p1.first > p2.first)
        {
            return true;
        }
        return false;
    });
    int rowIdx = 0;
    for(auto& row:algoOrders)
    {
        string algoId(row.second.AlgoOrderID);
        m_rowIdx2AlgoId.insert({rowIdx, {algoId, row.second.AlgoOrderType}});
        fillRow(rowIdx, row.second, &algoUserMgr);
        ++rowIdx;
    }
}

void AlgoOrderTab::showDetailOrders(QOrderMgr *orderMgr, int row, int col)
{
    if(row >= m_rowIdx2AlgoId.size()) return;
    auto twRef = m_rowIdx2AlgoId[row].first;

    ModelSingleOrder* pOrder = new ModelSingleOrder();
    pOrder->drawAlgoOrders(twRef, orderMgr);
    pOrder->setOrderToShow(ORDER_TO_SHOW::ALGO_TWAP);
    QTableView*         _tableview = new QTableView();
    _tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _tableview->setModel(pOrder);

    QHBoxLayout* h = new QHBoxLayout();
    h->addWidget(_tableview);

    TwapShowDetail* qWin = new TwapShowDetail();
    qWin->setContent(_tableview, pOrder, orderMgr);
    QString output = "TWAP 相关订单";
    output += ", ID ";
    output += twRef.c_str();
    qWin->setWindowTitle(output);
    qWin->setLayout(h);
    qWin->show();
}

void AlgoOrderTab::cancelTwapOrder(AlgoOrderMgrByUser &algoUserMgr, int row)
{
    string twRef = m_rowIdx2AlgoId[row].first;
    XTAlgoOrderType twTy = m_rowIdx2AlgoId[row].second;

    QString strMsg;
    strMsg.sprintf("是否对委托单<font color='red'>[%d]</font>进行撤单?", twRef.c_str());
    QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, "撤单提醒",strMsg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb != QMessageBox::Yes)
    {
        return;
    }

    algoUserMgr.cancelOrder(twRef, twTy);
}

void AlgoOrderTab::initControls()
{
    _algoOrderModel = new QStandardItemModel();
    this->setModel(_algoOrderModel);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setAlgoHeader();
}

void AlgoOrderTab::setAlgoHeader()
{
    QStandardItem* pItem = new QStandardItem();
    pItem->setText("ID Number");
    _algoOrderModel->setHorizontalHeaderItem(0, pItem);

    pItem = new QStandardItem();
    pItem->setText("Code");
    _algoOrderModel->setHorizontalHeaderItem(1, pItem);

    pItem = new QStandardItem();
    pItem->setText("Status");
    _algoOrderModel->setHorizontalHeaderItem(2, pItem);

    pItem = new QStandardItem();
    pItem->setText("ErrorMsg");
    _algoOrderModel->setHorizontalHeaderItem(3, pItem);

    pItem = new QStandardItem();
    pItem->setText("StartTime");
    _algoOrderModel->setHorizontalHeaderItem(4, pItem);


    pItem = new QStandardItem();
    pItem->setText("EndTime");
    _algoOrderModel->setHorizontalHeaderItem(5, pItem);

    pItem = new QStandardItem();
    pItem->setText("Offset");
    _algoOrderModel->setHorizontalHeaderItem(6, pItem);

    pItem = new QStandardItem();
    pItem->setText("BuyOrSell");
    _algoOrderModel->setHorizontalHeaderItem(7, pItem);


    pItem = new QStandardItem();
    pItem->setText("Volumn");
    _algoOrderModel->setHorizontalHeaderItem(8, pItem);

    pItem = new QStandardItem();
    pItem->setText("RemainVolumn");
    _algoOrderModel->setHorizontalHeaderItem(9, pItem);

    pItem = new QStandardItem();
    pItem->setText("Detail");
    _algoOrderModel->setHorizontalHeaderItem(10, pItem);
}

void AlgoOrderTab::fillRow(int row, tagXTAlgoOrderField &order, const AlgoOrderMgrByUser* pAlgoUser)
{
    QStandardItem* pItem = new QStandardItem();
    QString strID = order.AlgoOrderID;
    pItem->setText(strID);
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);

    _algoOrderModel->setItem(row, 0, pItem);

    pItem = new QStandardItem();
    QString strCode = order.ExCode.Code;
    pItem->setText(strCode);
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _algoOrderModel->setItem(row, 1, pItem);

    pItem = new QStandardItem();
    pItem->setText(algoStatus2Text(order.AlgoOrderStatus));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    if(algoStatus2Text(order.AlgoOrderStatus) == "执行中")
    {
       // pItem->setTextColor(QColor(236,41,103));
        pItem->setForeground(QBrush(QColor(236,41,103)));
    }
    _algoOrderModel->setItem(row, 2, pItem);

    pItem = new QStandardItem();
    pItem->setText(pAlgoUser->getErrorMsg(order.AlgoOrderID, 0));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _algoOrderModel->setItem(row, 3, pItem);

    QDateTime begin = QDateTime::fromTime_t((uint)order.StartTime);
    pItem = new QStandardItem();
    pItem->setText(begin.toString());
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _algoOrderModel->setItem(row, 4, pItem);

    QDateTime end = QDateTime::fromTime_t((uint)order.EndTime);
    pItem = new QStandardItem();
    pItem->setText(end.toString());
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _algoOrderModel->setItem(row, 5, pItem);


    pItem = new QStandardItem();
    pItem->setText(stool::openClose2Text(&order.Offset));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _algoOrderModel->setItem(row, 6, pItem);

    pItem = new QStandardItem();
    pItem->setText(stool::buySell2Text(order.Direction));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _algoOrderModel->setItem(row, 7, pItem);

    pItem = new QStandardItem();
    pItem->setText(QString::number(order.Volumn));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _algoOrderModel->setItem(row, 8, pItem);

    pItem = new QStandardItem();
    auto remainVol = order.Volumn - pAlgoUser->tradedOrders(order);
  //  if(order.AlgoOrderStatus == XT_AlGO_End)
  //  {
  //      remainVol = 0;
  //  }
    pItem->setText(QString::number(remainVol));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _algoOrderModel->setItem(row, 9, pItem);

    pItem = new QStandardItem();
    pItem->setText(tr("订单详情"));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _algoOrderModel->setItem(row, 10, pItem);
}

