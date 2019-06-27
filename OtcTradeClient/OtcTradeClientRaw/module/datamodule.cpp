#include "datamodule.h"
#include "datamoduleimpl.h"

QExchange::QExchange()
{

}

QExchange::~QExchange()
{

}

QDataModule::QDataModule()
{

}

QDataModule::~QDataModule()
{

}

QDataModule*        g_dataModule = nullptr;

QDataModule*        GetDataModule()
{
    if (nullptr == g_dataModule)
    {
        QDataModuleImpl* p = new QDataModuleImpl();
        g_dataModule = static_cast<QDataModule*>(p);
    }

    return  g_dataModule;
}
