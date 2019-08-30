#include "dataparkcondorder.h"
#include "trademodule.h"
#include "configmodule.h"
#include <vector>
#include "XTCodec.h"
#include <QDebug>
#include "parkordermgr.h"
#include "eventcentermodule.h"

using namespace std;

DataParkCondOrder::DataParkCondOrder(QObject *parent)
{
    _userSel = nullptr;
    _parkOrderView = nullptr;
    _condOrderView = nullptr;
    initControls();
}

DataParkCondOrder::~DataParkCondOrder()
{
    _userSel = nullptr;
    _parkOrderView = nullptr;
    _condOrderView = nullptr;
}

void DataParkCondOrder::updateParkOrderInfo(int orderType)
{
 //   ParkCondOrderMgrByUser* _curMgr = (ParkCondOrderMgrByUser *)v.value<void *>();
 //   shared_ptr<ParkCondOrderMgrByUser> parkUser;
 //   parkUser.reset(_curMgr);

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

    auto parkUser = GetParkedOrderMgr();

    if(parkUser)
    {
        auto user = parkUser->findMgrByUser(key);
        if(orderType == 0)
        {
            _parkOrderView->setUserMgr(user);
        }
        if(orderType == 1)
        {
            _condOrderView->setUserMgr(user);
        }
    }

}

void DataParkCondOrder::onClickedTableView(const QModelIndex &ind)
{
    qDebug() << "click row = " << ind.row() << " ,col = " << ind.column();
    if(_tabViews->currentIndex() == 0)
    {
        auto curRow = ind.row();
        auto parkId = _parkOrderView->findParkId(curRow);
        auto strkey = getCurParkUser();
        if(parkId.size()>0 && strkey.size()>0)
        {
            auto parkUser = GetParkedOrderMgr();

            if(parkUser)
            {
                auto user = parkUser->findMgrByUser(strkey);
                if(user)
                {
                    user->cancelParkOrder(parkId);
                }
            }
        }
    }
    else if(_tabViews->currentIndex() == 1)
    {
        auto curRow = ind.row();
        auto parkId = _condOrderView->findParkId(curRow);
        auto strkey = getCurParkUser();
        if(parkId.size()>0 && strkey.size()>0)
        {
            auto parkUser = GetParkedOrderMgr();

            if(parkUser)
            {
                auto user = parkUser->findMgrByUser(strkey);
                if(user)
                {
                    user->cancelCondOrder(parkId);
                }
            }
        }
    }
}

void DataParkCondOrder::onOrderEvent(ParkOrderEvent *pEvent)
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
                updateParkOrderInfo(pEvent->usertype);
                return;
            }
        }
    }
}

void DataParkCondOrder::initControls()
{
    QHBoxLayout* h1 = new QHBoxLayout();
    QLabel* account = new QLabel();
    account->setText(tr("账号:"));
    h1->addWidget(account);

    _parkOrderView = new ParkedOrderTab();
    _condOrderView = new CondOrderTab();

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
    _tabViews->addTab(_parkOrderView, tr("预置订单"));
    _tabViews->addTab(_condOrderView, tr("条件订单"));

    QObject::connect(_parkOrderView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onClickedTableView(const QModelIndex&)));
    QObject::connect(_condOrderView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onClickedTableView(const QModelIndex&)));

    QVBoxLayout* v1 = new QVBoxLayout();
    v1->addLayout(h1, 1);
    v1->addWidget(_tabViews);

    this->setLayout(v1);

    QObject::connect(GetEventCenter(), SIGNAL(fireParkOrderEvent(ParkOrderEvent*)), this, SLOT(onOrderEvent(ParkOrderEvent*)) );

    updateParkOrderInfo(0);
    updateParkOrderInfo(1);
}

string DataParkCondOrder::getCurParkUser()
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
    return key;
}
