#include "twaporderdata.h"
#include "trademodule.h"
#include "XTCodec.h"
#include "eventcentermodule.h"
#include "stool.h"
#include "twapshowdetail.h"
#include "trademodule.h"
#include "twapordermgr.h"

//.........................................................................................................
//.........................................................................................................
//.........................................................................................................
//.........................................................................................................

CTwapOrderData::CTwapOrderData(QWidget *parent):
    QWidget(parent)
{
    _twapOrderTab = nullptr;
    _tabViews = nullptr;
    _userSel = nullptr;
    initControls();
}

void CTwapOrderData::updateTwapOrderInfo()
{
    vector<tagTTradeUserParam> arrUser;
    GetConfigModule()->GetAllTradeUser(arrUser);
    char strkey[255]={0};

    for(auto& user:arrUser)
    {
        QString alias(user.aliasuser);
        if(alias == _userSel->currentText())
        {
            sprintf(strkey, "%d_%s", user.broker, user.user);
            break;
        }
    }
    string key(strkey);

    auto twapMgr = GetTwapOrderMgr();

    if(twapMgr)
    {
        auto user = twapMgr->findOrderMgr(key);
        
        _twapOrderTab->setUserMgr(user);
    }
}

void CTwapOrderData::onDoubleClickedTableView(const QModelIndex &ind)
{
    int row = ind.row();
    int col = ind.column();

    QVariant v = _userSel->currentData();
    QOrderMgr* user = (QOrderMgr *)v.value<void *>();
    if(nullptr == user)
    {
        return;
    }

    if(col == 10)
    {
        _twapOrderTab->showDetailOrders(user, row, col);
    }
    else
    {
        vector<tagTTradeUserParam> arrUser;
        GetConfigModule()->GetAllTradeUser(arrUser);
        char strkey[255]{0};

        for(auto& user:arrUser)
        {
            QString alias(user.aliasuser);
            if(alias == _userSel->currentText())
            {
                sprintf(strkey, "%d_%s", user.broker, user.user);
                break;
            }
        }
        string key(strkey);

        auto twapMgr = GetTwapOrderMgr();
        if(twapMgr)
        {
            auto user = twapMgr->findOrderMgr(key);
            if(user)
            {
                _twapOrderTab->cancelTwapOrder(user, row);
            }
        }
    }
}

void CTwapOrderData::onOrderEvent(TwapOrderEvent *pEvent)
{
    vector<tagTTradeUserParam> arrUser;
    GetConfigModule()->GetAllTradeUser(arrUser);

    for(auto& user:arrUser)
    {
        QString alias(user.aliasuser);
        if(alias == _userSel->currentText())
        {
            if(user.broker == pEvent->broker && strncmp(user.user, pEvent->user, strlen(user.user)) == 0)
            {
                updateTwapOrderInfo();
                return;
            }
        }
    }
}

void CTwapOrderData::onInsertOrderEvent(OrderEventArgs *pEvent)
{
    vector<tagTTradeUserParam> arrUser;
    GetConfigModule()->GetAllTradeUser(arrUser);

   // if(pEvent->subevent != ORDER_EVENT_UPDATE) return;
    for(auto& user:arrUser)
    {
        QString alias(user.aliasuser);
        if(alias == _userSel->currentText())
        {
            if(user.broker == pEvent->broker && strncmp(user.user, pEvent->user, strlen(user.user)) == 0)
            {
                updateTwapOrderInfo();
                return;
            }
        }
    }
}

void CTwapOrderData::on_combox_indexChange(int index)
{
    updateTwapOrderInfo();
}

void CTwapOrderData::initControls()
{
    QHBoxLayout* h1 = new QHBoxLayout();
    QLabel* account = new QLabel();
    account->setText(tr("账号:"));
    h1->addWidget(account);

    _twapOrderTab = new TwapOrderTable();

    _userSel = new QComboBox();
    vector<tagTTradeUserParam> arrUser;
    GetConfigModule()->GetAllTradeUser(arrUser);
    int ncount = arrUser.size();
    for (int i = 0; i < ncount; i++)
    {
        tagTTradeUserParam& o = arrUser[i];

        QOrderMgr* p = GetTradeModule()->GetMgr(o.type, o.broker, o.user);

        char key[255]{0};
        sprintf(key, "%d_%s", o.broker, o.user);
        string strkey(key);


        QString strName;
        std::string strmsg = o.aliasuser;
        strName = XTCodec::AfGbk_ToQString(strmsg);

        QVariant v = QVariant::fromValue((void *)p);
        _userSel->addItem(strName, v);
    }

    h1->addWidget(_userSel);
    h1->addStretch();

    _tabViews = new QTabWidget();
    _tabViews->addTab(_twapOrderTab, tr("TWAP订单"));

    QObject::connect(_twapOrderTab, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onDoubleClickedTableView(const QModelIndex&)));

    QVBoxLayout* v1 = new QVBoxLayout();
    v1->addLayout(h1, 1);
    v1->addWidget(_tabViews);

    this->setLayout(v1);

    QObject::connect(GetEventCenter(), SIGNAL(fireTwapTwapOrderEvent(TwapOrderEvent*)), this, SLOT(onOrderEvent(TwapOrderEvent*)) );
    QObject::connect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onInsertOrderEvent(OrderEventArgs*)) );
    QObject::connect(_userSel, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combox_indexChange(int)));
    updateTwapOrderInfo();
}

//.........................................................................................................
//.........................................................................................................
//.........................................................................................................
//.........................................................................................................
inline QString twapStatus2Text(char status)
{
    QString result;
    if(status == XT_TWAP_Accepted)
    {
        result = "已报";
    }
    else if(status == XT_TWAP_End)
    {
        result = "执行结束";
    }
    else if(status == XT_TWAP_Cancelled)
    {
        result = "已撤";
    }
    else if(status == XT_TWAP_Executing)
    {
        result = "执行中";
    }
    else if (status == XT_TWAP_AVOID)
    {
        result = "废单";
    }
    else
    {
        result = "未知";
    }
    
    return result;
}

TwapOrderTable::TwapOrderTable(QWidget *parent)
{
    _twapOrderModel = nullptr;
    initControls();
}

void TwapOrderTable::setUserMgr(TwapOrderMgrByUser* twapUserMgr)
{
    m_rowIdx2TwapId.clear();
    if(!twapUserMgr)
    {
        return;
    }
    auto twapOrders = twapUserMgr->getAllTwapOrders();
    if(twapOrders.size()<=0)
    {
        _twapOrderModel->removeRows(0, _twapOrderModel->rowCount());
    }
    sort(twapOrders.begin(), twapOrders.end(), [](const auto& p1, const auto& p2)
    {
        if(p1.first > p2.first)
        {
            return true;
        }
        return false;
    });
    int rowIdx = 0;
    for(auto& row:twapOrders)
    {
        string twapId(row.second.TwapOrderID);
        m_rowIdx2TwapId.insert({rowIdx, {twapId, row.second.TwapOrderType}});
        fillRow(rowIdx, row.second, twapUserMgr);
        ++rowIdx;
    }
}

void TwapOrderTable::showDetailOrders(QOrderMgr *orderMgr, int row, int col)
{
    if(row >= m_rowIdx2TwapId.size())
    {
        return;
    }

    auto twRef = m_rowIdx2TwapId[row].first;

    CTwapShowDetail* pWin = new CTwapShowDetail();
    pWin->initData(twRef, orderMgr);
    pWin->show();

}

void TwapOrderTable::cancelTwapOrder(TwapOrderMgrByUser* twapUserMgr, int row)
{
    string twRef = m_rowIdx2TwapId[row].first;
    char twTy = m_rowIdx2TwapId[row].second;

    QString strMsg;
    strMsg.sprintf("是否对Twap单<font color='red'>[%s]</font>进行撤单?", twRef.c_str());
    QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, "撤单提醒",strMsg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb != QMessageBox::Yes)
    {
        return;
    }

    twapUserMgr->DoReqTwapOrderAction(twRef, twTy);
}

void    TwapOrderTable::cancelAllTwapOrder(TwapOrderMgrByUser* twapUserMgr)
{
    std::unordered_map<int, std::pair<std::string, char>>::iterator itb = m_rowIdx2TwapId.begin();
    while(itb != m_rowIdx2TwapId.end())
    {
        twapUserMgr->DoReqTwapOrderAction(itb->second.first, itb->second.second);

        itb++;
    }
}

void TwapOrderTable::initControls()
{
    _twapOrderModel = new QStandardItemModel();
    this->setModel(_twapOrderModel);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setTwapHeader();
}

void TwapOrderTable::setTwapHeader()
{
    QStandardItem* pItem = new QStandardItem();
    pItem->setText("编号");
    _twapOrderModel->setHorizontalHeaderItem(0, pItem);

    pItem = new QStandardItem();
    pItem->setText("合约");
    _twapOrderModel->setHorizontalHeaderItem(1, pItem);

    pItem = new QStandardItem();
    pItem->setText("状态");
    _twapOrderModel->setHorizontalHeaderItem(2, pItem);

    pItem = new QStandardItem();
    pItem->setText("错误信息");
    _twapOrderModel->setHorizontalHeaderItem(3, pItem);

    pItem = new QStandardItem();
    pItem->setText("开始时间");
    _twapOrderModel->setHorizontalHeaderItem(4, pItem);

    pItem = new QStandardItem();
    pItem->setText("结束时间");
    _twapOrderModel->setHorizontalHeaderItem(5, pItem);

    pItem = new QStandardItem();
    pItem->setText("开平");
    _twapOrderModel->setHorizontalHeaderItem(6, pItem);

    pItem = new QStandardItem();
    pItem->setText("方向");
    _twapOrderModel->setHorizontalHeaderItem(7, pItem);

    pItem = new QStandardItem();
    pItem->setText("数量");
    _twapOrderModel->setHorizontalHeaderItem(8, pItem);

    pItem = new QStandardItem();
    pItem->setText("剩余量");
    _twapOrderModel->setHorizontalHeaderItem(9, pItem);

    pItem = new QStandardItem();
    pItem->setText("详细信息");
    _twapOrderModel->setHorizontalHeaderItem(10, pItem);

    pItem = new QStandardItem();
    pItem->setText("状态信息");
    _twapOrderModel->setHorizontalHeaderItem(11, pItem);

    pItem = new QStandardItem();
    pItem->setText("RemainVol(ref)");
    _twapOrderModel->setHorizontalHeaderItem(12, pItem);

    pItem = new QStandardItem();
    pItem->setText("RealStartTime");
    _twapOrderModel->setHorizontalHeaderItem(13, pItem);

    pItem = new QStandardItem();
    pItem->setText("RealEndTime");
    _twapOrderModel->setHorizontalHeaderItem(14, pItem);

}

void TwapOrderTable::fillRow(int row, tagXTTwapOrderField &order, const TwapOrderMgrByUser* pTwapUser)
{
    //[0]TwapID
    QStandardItem* pItem = new QStandardItem();
    QString strID = order.TwapOrderID;
    pItem->setText(strID);
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);

    _twapOrderModel->setItem(row, 0, pItem);

    //[1]Code
    pItem = new QStandardItem();
    QString strCode = order.ExCode.Code;
    pItem->setText(strCode);
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _twapOrderModel->setItem(row, 1, pItem);

    //[2]Status
    pItem = new QStandardItem();
    pItem->setText(twapStatus2Text(order.TwapOrderStatus));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    if(order.TwapOrderStatus == XT_TWAP_Executing)
    {
        pItem->setForeground(QBrush(QColor(236,41,103)));
    }
    _twapOrderModel->setItem(row, 2, pItem);

    //[3]ErrorMsg
    pItem = new QStandardItem();
    pItem->setText(pTwapUser->getErrorMsg(order.TwapOrderID, 0));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _twapOrderModel->setItem(row, 3, pItem);

    //[4]开始时间
    pItem = new QStandardItem();
    pItem->setText(QString::number(order.BeginTime));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _twapOrderModel->setItem(row, 4, pItem);

    //[5]结束时间
    pItem = new QStandardItem();
    pItem->setText(QString::number(order.EndTime));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _twapOrderModel->setItem(row, 5, pItem);

    //[6]开平标志
    pItem = new QStandardItem();
    pItem->setText(stool::openClose2Text(&order.OffsetFlag));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _twapOrderModel->setItem(row, 6, pItem);

    //[7]方向
    pItem = new QStandardItem();
    pItem->setText(stool::buySell2Text(order.Direction));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _twapOrderModel->setItem(row, 7, pItem);

    //[8]数量
    pItem = new QStandardItem();
    pItem->setText(QString::number(order.Volume));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _twapOrderModel->setItem(row, 8, pItem);

    //[9]剩余数量(交易)
    pItem = new QStandardItem();
    std::string twapid(order.TwapOrderID);
    auto remainVol = order.Volume - pTwapUser->tradedOrders(twapid);

    pItem->setText(QString::number(remainVol));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _twapOrderModel->setItem(row, 9, pItem);

    //[10]下委托单详情
    pItem = new QStandardItem();
    pItem->setText(tr("订单详情"));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _twapOrderModel->setItem(row, 10, pItem);

    //[11]状态信息
    if( strlen(order.StatusMsg) > 0 )
    {
        pItem = new QStandardItem();
        QString strText = order.StatusMsg;
        pItem->setText(strText);
        pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
        _twapOrderModel->setItem(row, 11, pItem);
    }

    //[12]剩余数量(委托)
    pItem = new QStandardItem();
    pItem->setText(QString::number(order.RemainVolume));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _twapOrderModel->setItem(row, 12, pItem);

    //[13]真实的开始时间
    pItem = new QStandardItem();
    pItem->setText(QString::number(order.RealStartTime));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _twapOrderModel->setItem(row, 13, pItem);

    //[14]真实的结束时间
    pItem = new QStandardItem();
    pItem->setText(QString::number(order.RealEndTime));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _twapOrderModel->setItem(row, 14, pItem);
}










