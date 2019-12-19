
#include "parkorderdata.h"
#include "trademodule.h"
#include "XTCodec.h"
#include "eventcentermodule.h"
#include "stool.h"
#include "trademodule.h"
#include "parkordermgr.h"

//.........................................................................................................
//.........................................................................................................
//.........................................................................................................
//.........................................................................................................

CParkOrderData::CParkOrderData(QWidget *parent):
    QWidget(parent)
{
    _parkOrderTab = nullptr;
    _tabViews = nullptr;
    _userSel = nullptr;
    initControls();
}

void CParkOrderData::updateParkOrderInfo()
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

    auto parkMgr = GetParkedOrderMgr();

    if(parkMgr)
    {
        ParkCondOrderMgrByUser* user = parkMgr->findMgrByUser(key);

        _parkOrderTab->setUserMgr(user);
    }
}

void CParkOrderData::onDoubleClickedTableView(const QModelIndex &ind)
{
    int row = ind.row();
    //int col = ind.column();

    QVariant v = _userSel->currentData();
    QOrderMgr* user = (QOrderMgr *)v.value<void *>();
    if(nullptr == user)
    {
        return;
    }

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

        auto parkMgr = GetParkedOrderMgr();
        if(parkMgr)
        {
            auto user = parkMgr->findMgrByUser(key);
            if(user)
            {
                _parkOrderTab->cancelParkOrder(user, row);
            }
        }
    }
}

void CParkOrderData::onParkOrderEvent(ParkOrderEvent* pEvent)
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
                updateParkOrderInfo();
                return;
            }
        }
    }
}

void CParkOrderData::on_combox_indexChange(int /*index*/)
{
    updateParkOrderInfo();
}

void CParkOrderData::initControls()
{
    QHBoxLayout* h1 = new QHBoxLayout();
    QLabel* account = new QLabel();
    account->setText(tr("账号:"));
    h1->addWidget(account);

    _parkOrderTab = new ParkOrderTable();

    _userSel = new QComboBox();
    vector<tagTTradeUserParam> arrUser;
    GetConfigModule()->GetAllTradeUser(arrUser);
    size_t ncount = arrUser.size();
    for (size_t i = 0; i < ncount; i++)
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
    _tabViews->addTab(_parkOrderTab, tr("Park订单"));

    QObject::connect(_parkOrderTab, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onDoubleClickedTableView(const QModelIndex&)));

    QVBoxLayout* v1 = new QVBoxLayout();
    v1->addLayout(h1, 1);
    v1->addWidget(_tabViews);

    this->setLayout(v1);

    QObject::connect(GetEventCenter(), SIGNAL(fireParkOrderEvent(ParkOrderEvent*)), this, SLOT(onParkOrderEvent(ParkOrderEvent*)) );
    QObject::connect(_userSel, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combox_indexChange(int)));

    updateParkOrderInfo();
}

//.........................................................................................................
//.........................................................................................................
//.........................................................................................................
//.........................................................................................................
inline QString parkStatus2Text(char status)
{
    QString result;
    if(status == XT_PAOS_NotSend)
    {
        result = "未执行";
    }
    else if(status == XT_PAOS_Send)
    {
        result = "已发送";
    }
    else if(status == XT_PAOS_Deleted)
    {
        result = "已撤";
    }
    else
    {
        result = "未知";
    }

    return result;
}

inline QString parkType2Text(char type)
{
    QString result;
    if(type == XT_PT_CONDITION)
    {
        result = "条件单";
    }
    else if(type == XT_PT_EXCHCONTINUE)
    {
        result = "连续竞价";
    }
    else if(type == XT_PT_EXCHAUCTION)
    {
        result = "集合竞价";
    }
    else if(type == XT_PT_TIME)
    {
        result = "定时预埋";
    }
    else if(type == XT_PT_HUMAN)
    {
        result = "预埋手工";
    }
    else
    {
        result = "未知";
    }

    return result;
}

ParkOrderTable::ParkOrderTable(QWidget *parent)
    :QTableView(parent)
{
    _parkOrderModel = nullptr;
    initControls();
}

void ParkOrderTable::setUserMgr(ParkCondOrderMgrByUser* parkUserMgr)
{
    m_rowIdx2ParkId.clear();
    if(!parkUserMgr)
    {
        return;
    }

    vector<tagXTParkedOrderField> parkOrders;
    parkUserMgr->getAllParkorder( parkOrders );
    if(parkOrders.size()<=0)
    {
        _parkOrderModel->removeRows(0, _parkOrderModel->rowCount());
    }

    int rowIdx = 0;
    for(auto& row:parkOrders)
    {
        string parkId(row.ParkedOrderID);
        m_rowIdx2ParkId.insert({rowIdx, {parkId, row.ParkedType}});
        fillRow(rowIdx, row);
        ++rowIdx;
    }
}

void ParkOrderTable::cancelParkOrder(ParkCondOrderMgrByUser* parkUserMgr, int row)
{
    string parkId = m_rowIdx2ParkId[row].first;

    QString strMsg;
    strMsg.sprintf("是否对Park单<font color='red'>[%s]</font>进行撤单?", parkId.c_str());
    QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, "撤单提醒",strMsg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(rb != QMessageBox::Yes)
    {
        return;
    }

    parkUserMgr->cancelParkOrder(parkId);
}

void    ParkOrderTable::manualSendParkOrder(ParkCondOrderMgrByUser* parkUserMgr, int row)
{
    string parkId = m_rowIdx2ParkId[row].first;

    parkUserMgr->manualSendOrder(parkId);
}

void    ParkOrderTable::cancelAllParkOrder(ParkCondOrderMgrByUser* parkUserMgr)
{
    std::unordered_map<int, std::pair<std::string, char>>::iterator itb = m_rowIdx2ParkId.begin();
    while(itb != m_rowIdx2ParkId.end())
    {
        parkUserMgr->cancelParkOrder(itb->second.first);

        itb++;
    }
}

void ParkOrderTable::initControls()
{
    _parkOrderModel = new QStandardItemModel();
    this->setModel(_parkOrderModel);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setParkHeader();
}

void ParkOrderTable::setParkHeader()
{
    QStandardItem* pItem = new QStandardItem();
    pItem->setText("编号");
    _parkOrderModel->setHorizontalHeaderItem(0, pItem);

    pItem = new QStandardItem();
    pItem->setText("类型");
    _parkOrderModel->setHorizontalHeaderItem(1, pItem);

    pItem = new QStandardItem();
    pItem->setText("状态");
    _parkOrderModel->setHorizontalHeaderItem(2, pItem);

    pItem = new QStandardItem();
    pItem->setText("合约");
    _parkOrderModel->setHorizontalHeaderItem(3, pItem);

    pItem = new QStandardItem();
    pItem->setText("方向");
    _parkOrderModel->setHorizontalHeaderItem(4, pItem);

    pItem = new QStandardItem();
    pItem->setText("开平");
    _parkOrderModel->setHorizontalHeaderItem(5, pItem);

    pItem = new QStandardItem();
    pItem->setText("价格");
    _parkOrderModel->setHorizontalHeaderItem(6, pItem);

    pItem = new QStandardItem();
    pItem->setText("数量");
    _parkOrderModel->setHorizontalHeaderItem(7, pItem);

    pItem = new QStandardItem();
    pItem->setText("条件描述");
    _parkOrderModel->setHorizontalHeaderItem(8, pItem);

    pItem = new QStandardItem();
    pItem->setText("消息备注");
    _parkOrderModel->setHorizontalHeaderItem(9, pItem);

    pItem = new QStandardItem();
    pItem->setText("下单时间");
    _parkOrderModel->setHorizontalHeaderItem(10, pItem);

    pItem = new QStandardItem();
    pItem->setText("下单日期");
    _parkOrderModel->setHorizontalHeaderItem(11, pItem);

    pItem = new QStandardItem();
    pItem->setText("交易日");
    _parkOrderModel->setHorizontalHeaderItem(12, pItem);

    pItem = new QStandardItem();
    pItem->setText("套保");
    _parkOrderModel->setHorizontalHeaderItem(13, pItem);

}

void ParkOrderTable::fillRow(int row, tagXTParkedOrderField &order)
{
    //[0]ParkID
    QStandardItem* pItem = new QStandardItem();
    QString strID = order.ParkedOrderID;
    pItem->setText(strID);
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);

    _parkOrderModel->setItem(row, 0, pItem);


    //[1]类型
    pItem = new QStandardItem();
    pItem->setText(parkType2Text(order.ParkedType));
    _parkOrderModel->setItem(row, 1, pItem);

    //[2]状态
    pItem = new QStandardItem();
    pItem->setText(parkStatus2Text(order.ParkedStatus));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    if(order.ParkedStatus == XT_PAOS_Send)
    {
        pItem->setForeground(QBrush(QColor(236,41,103)));
    }
    _parkOrderModel->setItem(row, 2, pItem);

    //[3]合约
    pItem = new QStandardItem();
    QString strCode = order.ExCode.Code;
    pItem->setText(strCode);
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkOrderModel->setItem(row, 3, pItem);

    //[4]方向
    pItem = new QStandardItem();
    pItem->setText(stool::buySell2Text(order.Direction));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkOrderModel->setItem(row, 4, pItem);

    //[5]开平标志
    pItem = new QStandardItem();
    pItem->setText(stool::openClose2Text(&order.OffsetFlag[0]));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkOrderModel->setItem(row, 5, pItem);


    //[6]价格
    pItem = new QStandardItem();
    pItem->setText(QString::number(order.LimitPrice));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkOrderModel->setItem(row, 6, pItem);

    //[7]数量
    pItem = new QStandardItem();
    pItem->setText(QString::number(order.Volume));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkOrderModel->setItem(row, 7, pItem);

    //[8]条件描述
    pItem = new QStandardItem();
    {
        QString strText;
        switch (order.ParkedType)
        {
        case XT_PT_CONDITION:
        {
            switch (order.ConditionType)
            {
                case XT_CC_LastPriceGreaterThanStopPrice: strText.sprintf("最新价 > [%.04lf]", order.StopPrice);break;
                case XT_CC_LastPriceGreaterEqualStopPrice: strText.sprintf("最新价 >= [%.04lf]", order.StopPrice);break;
                case XT_CC_LastPriceLesserThanStopPrice: strText.sprintf("最新价 < [%.04lf]", order.StopPrice);break;
                case XT_CC_LastPriceLesserEqualStopPrice: strText.sprintf("最新价 <= [%.04lf]", order.StopPrice);break;

                case XT_CC_AskPriceGreaterThanStopPrice:strText.sprintf("卖一价 > [%.04lf]", order.StopPrice);break;
                case XT_CC_AskPriceGreaterEqualStopPrice:strText.sprintf("卖一价 >= [%.04lf]", order.StopPrice);break;
                case XT_CC_AskPriceLesserThanStopPrice:strText.sprintf("卖一价 < [%.04lf]", order.StopPrice);break;
                case XT_CC_AskPriceLesserEqualStopPrice:strText.sprintf("卖一价 <= [%.04lf]", order.StopPrice);break;

                case XT_CC_BidPriceGreaterThanStopPrice:strText.sprintf("买一价 > [%.04lf]", order.StopPrice);break;
                case XT_CC_BidPriceGreaterEqualStopPrice:strText.sprintf("买一价 >= [%.04lf]", order.StopPrice);break;
                case XT_CC_BidPriceLesserThanStopPrice:strText.sprintf("买一价 < [%.04lf]", order.StopPrice);break;
                case XT_CC_BidPriceLesserEqualStopPrice:strText.sprintf("买一价 <= [%.04lf]", order.StopPrice);break;
                default:strText = "未知条件"; break;
            }
        }
            break;
        case XT_PT_EXCHAUCTION:
            strText = "进入集合竞价时";
            break;
        case XT_PT_EXCHCONTINUE:
            strText = "首次进入连续竞价时";
            break;
        case XT_PT_TIME:
            strText.sprintf("定时发送[%d]", order.StopTime);
            break;
        case XT_PT_HUMAN:
            strText = "手工发送";
            break;
        default:
            strText="未知";
            break;
        }
        pItem->setText(strText);
    }
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkOrderModel->setItem(row, 8, pItem);

    //[9]消息备注
    if( strlen(order.StatusMsg) > 0 )
    {
        pItem = new QStandardItem();
        QString strText = order.StatusMsg;
        pItem->setText(strText);
        pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
        _parkOrderModel->setItem(row, 9, pItem);
    }

    //[10]下单时间
    pItem = new QStandardItem();
    pItem->setText(QString::number(order.InsertTime));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkOrderModel->setItem(row, 10, pItem);

    //[11]下单日期
    pItem = new QStandardItem();
    pItem->setText(QString::number(order.InsertDate));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkOrderModel->setItem(row, 11, pItem);

    //[12]交易日
    pItem = new QStandardItem();
    pItem->setText(QString::number(order.TradingDate));
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkOrderModel->setItem(row, 12, pItem);

    //[13]套保
    pItem = new QStandardItem();
    pItem->setText( stool::hedge2Text(order.HedgeFlag[0]) );
    pItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    _parkOrderModel->setItem(row, 13, pItem);

}



































