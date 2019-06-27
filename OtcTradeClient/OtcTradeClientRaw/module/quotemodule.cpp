

#include "quotemodule.h"
#include "quotemoduleimpl.h"

double GetCurrentPrice(tagXTInstrument& curCode)
{
    tagXTSnapshot quoteSnap;
    auto pQuoteMod = GetQuoteModule();
    double curPrice = 0.01;
    if(pQuoteMod)
    {
        auto res = pQuoteMod->GetSnapShot(curCode, &quoteSnap);
        if(res>0)
        {
            curPrice = quoteSnap.LastPrice;
        }
        else
        {
            pQuoteMod->SetCodeList(&curCode, 1);
        }
    }
    return curPrice;
}

QQuoteModule::QQuoteModule()
{

}

QQuoteModule::~QQuoteModule()
{

}

QQuoteModule*        g_quoteModule = nullptr;

QQuoteModule*        GetQuoteModule()
{
    if (nullptr == g_quoteModule)
    {
        QQuoteModuleImpl* p = new QQuoteModuleImpl();
        g_quoteModule = static_cast<QQuoteModule*>(p);
    }

    return  g_quoteModule;
}
