#include "trademodule.h"
#include "trademoduleimpl.h"

QOrderMgr::QOrderMgr()
{

}

QOrderMgr::~QOrderMgr()
{

}


QTradeModule::QTradeModule()
{

}

QTradeModule::~QTradeModule()
{

}

QTradeModule*        g_tradeModule = nullptr;

QTradeModule*        GetTradeModule()
{
    if (nullptr == g_tradeModule)
    {
        QTradeModuleImpl* p = new QTradeModuleImpl();
        g_tradeModule = static_cast<QTradeModule*>(p);
    }

    return  g_tradeModule;
}
