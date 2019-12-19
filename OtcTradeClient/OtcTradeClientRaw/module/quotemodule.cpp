
#include "quotemodule.h"
#include "quotemoduleimpl.h"

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
