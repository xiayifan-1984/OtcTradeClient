#include "dataparkcondorder.h"
#include "trademodule.h"
#include "configmodule.h"
#include <vector>
#include "XTCodec.h"
#include <QDebug>
#include "parkordermgr.h"

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

        //QOrderMgr* p = GetTradeModule()->GetMgr(o.type, o.broker, o.user);

        char key[255]{0};
        sprintf(key, "%d_%s", o.broker, o.user);
        string strkey(key);

        shared_ptr<ParkOrderMgrByUser> p = nullptr;
        auto pPark = GetParkedOrderMgr();
        if(pPark)
        {
            p = pPark->findMgrByUser(strkey);
        }

        QString strName;
        std::string strmsg = o.aliasuser;
        strName = XTCodec::AfGbk_ToQString(strmsg);

        QVariant v = QVariant::fromValue((void *)p.get());
        _userSel->addItem(strName, v);
    }


    h1->addWidget(_userSel);
    h1->addStretch();

    _tabViews = new QTabWidget();
    _tabViews->addTab(_parkOrderView, tr("预置订单"));
    _tabViews->addTab(_condOrderView, tr("条件订单"));

    QVBoxLayout* v1 = new QVBoxLayout();
    v1->addLayout(h1, 1);
    v1->addWidget(_tabViews);

    this->setLayout(v1);
}
