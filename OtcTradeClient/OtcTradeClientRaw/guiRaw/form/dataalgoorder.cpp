#include "dataalgoorder.h"
#include "ordermgrimpl.h"
#include "XTCodec.h"


DataAlgoOrder::DataAlgoOrder(QObject *parent)
{
    _algoOrderTab = nullptr;
    _tabViews = nullptr;
    _userSel = nullptr;
    initControls();
}

void DataAlgoOrder::updateAlgoOrderInfo(XTAlgoOrderType algoType)
{
    vector<tagOneTradeUser> arrUser;
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

    auto algoMgr = GetAlgoOrderMgr();

    if(algoMgr)
    {
        auto user = algoMgr->findOrderMgr(key);
        if(algoType == XT_ALGO_TWAP)
        {
            _algoOrderTab->setUserMgr(*user);
        }
    }
}

void DataAlgoOrder::onClickedTableView(const QModelIndex &ind)
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
        _algoOrderTab->showDetailOrders(user, row, col);
    }
    else
    {
        vector<tagOneTradeUser> arrUser;
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

        auto algoMgr = GetAlgoOrderMgr();
        if(algoMgr)
        {
            auto user = algoMgr->findOrderMgr(key);
            if(user)
            {
                _algoOrderTab->cancelTwapOrder(*user, row);
            }
        }
    }
}

void DataAlgoOrder::onOrderEvent(AlgoOrderEvent *pEvent)
{
    vector<tagOneTradeUser> arrUser;
    GetConfigModule()->GetAllTradeUser(arrUser);

    for(auto& user:arrUser)
    {
        QString alias(user.aliasuser);
        if(alias == _userSel->currentText())
        {
            if(user.broker == pEvent->broker && strncmp(user.user, pEvent->user, strlen(user.user)) == 0)
            {
                updateAlgoOrderInfo(XT_ALGO_TWAP);
                return;
            }
        }
    }
}

void DataAlgoOrder::onInsertOrderEvent(OrderEventArgs *pEvent)
{
    vector<tagOneTradeUser> arrUser;
    GetConfigModule()->GetAllTradeUser(arrUser);

   // if(pEvent->subevent != ORDER_EVENT_UPDATE) return;
    for(auto& user:arrUser)
    {
        QString alias(user.aliasuser);
        if(alias == _userSel->currentText())
        {
            if(user.broker == pEvent->broker && strncmp(user.user, pEvent->user, strlen(user.user)) == 0)
            {
                updateAlgoOrderInfo(XT_ALGO_TWAP);
                return;
            }
        }
    }
}

void DataAlgoOrder::on_combox_indexChange(int index)
{
    updateAlgoOrderInfo(XT_ALGO_TWAP);
}

void DataAlgoOrder::initControls()
{
    QHBoxLayout* h1 = new QHBoxLayout();
    QLabel* account = new QLabel();
    account->setText(tr("账号:"));
    h1->addWidget(account);

    _algoOrderTab = new AlgoOrderTab();

    _userSel = new QComboBox();
    vector<tagOneTradeUser> arrUser;
    GetConfigModule()->GetAllTradeUser(arrUser);
    int ncount = arrUser.size();
    for (int i = 0; i < ncount; i++)
    {
        tagOneTradeUser& o = arrUser[i];

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
    _tabViews->addTab(_algoOrderTab, tr("TWAP订单"));

    QObject::connect(_algoOrderTab, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onClickedTableView(const QModelIndex&)));

    QVBoxLayout* v1 = new QVBoxLayout();
    v1->addLayout(h1, 1);
    v1->addWidget(_tabViews);

    this->setLayout(v1);

    QObject::connect(GetEventCenter(), SIGNAL(fireAlgoTwapOrderEvent(AlgoOrderEvent*)), this, SLOT(onOrderEvent(AlgoOrderEvent*)) );
    QObject::connect(GetEventCenter(), SIGNAL(fireOrderEvent(OrderEventArgs*)), this, SLOT(onInsertOrderEvent(OrderEventArgs*)) );
    QObject::connect(_userSel, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combox_indexChange(int)));
    updateAlgoOrderInfo(XT_ALGO_TWAP);
}

AlgoOrderMgrByUser *DataAlgoOrder::getCurrentUser()
{
    vector<tagOneTradeUser> arrUser;
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

    auto algoMgr = GetAlgoOrderMgr();

    if(algoMgr)
    {
        auto user = algoMgr->findOrderMgr(key);
        return user;
    }
    return nullptr;
}
