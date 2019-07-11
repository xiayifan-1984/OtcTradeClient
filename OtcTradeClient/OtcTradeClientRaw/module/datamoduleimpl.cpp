#include "datamoduleimpl.h"
#include "exchangeimpl.h"
#include <string.h>

QDataModuleImpl::QDataModuleImpl()
{
	memset(m_szDataDir, 0, sizeof(m_szDataDir));
}

int         QDataModuleImpl::Init(const char* pszDataDir)
{
	strcpy(m_szDataDir, pszDataDir);

	return 1;
}
void        QDataModuleImpl::Free()
{
    EndWork();
}

int         QDataModuleImpl::StartWork()
{
	const int  arrId[5] =
	{
		XT_EXCH_CFFEX, XT_EXCH_SHFE, XT_EXCH_CZCE,  XT_EXCH_DCE,  XT_EXCH_INE
	};
	const char* arrTitle[5] =
	{
		"CFFEX","SHFE","CZCE","DCE","INE"
	};

	char szCodeFile[255] = { 0 };
	char szProductFile[255] = { 0 };
	for (int i = 0; i < 5; i++)
	{
		memset(szCodeFile, 0, sizeof(szCodeFile));
		memset(szProductFile, 0, sizeof(szProductFile));
		snprintf(szCodeFile, 254, "%s_code.csv", arrTitle[i]);
		snprintf(szProductFile, 254, "%s_product.csv", arrTitle[i]);

		QExchangeImpl* p = new QExchangeImpl();
		p->Init(arrId[i]);
		p->ReadFile(m_szDataDir, szCodeFile, szProductFile);

		m_arrExch.push_back(static_cast<QExchange*>(p));
	}

    initCodeList();
	return 1;
}

void        QDataModuleImpl::EndWork()
{
	size_t nsize = m_arrExch.size();
	for (size_t i = 0; i < nsize; i++)
	{
		QExchange* p = m_arrExch[i];
		if (p)
		{
			QExchangeImpl* q = dynamic_cast<QExchangeImpl*>(p);
			q->Free();

			delete q;
		}
	}

	m_arrExch.clear();
}

int         QDataModuleImpl::GetExchangeList(std::vector<QExchange*>& arr)
{
	size_t count = m_arrExch.size();
	for (size_t i = 0; i < count; i++)
	{
		arr.push_back(m_arrExch[i]);
	}

	return count;
}

int         QDataModuleImpl::VerifyExCode(tagXTInstrument& inout)
{
    if(inout.ExchID != XT_EXCH_UNKONW)
    {
        return 1;
    }

    int ncount = m_arrExch.size();
    for(int i=0; i<ncount; i++)
    {
        if(  m_arrExch[i]->IsExistCode(inout) )
        {
            inout.ExchID = m_arrExch[i]->GetExchID();
            return 2;
        }
    }

    return (-1);
}

QExchange*  QDataModuleImpl::GetExchange(int exchid)
{
    int ncount = m_arrExch.size();
    for(int i=0; i<ncount; i++)
    {
        if(  m_arrExch[i]->GetExchID() == exchid )
        {
            return m_arrExch[i];
        }
    }

    return nullptr;
}

int         QDataModuleImpl::GetOneName(tagXTInstrument& oExCode, char* pname)
{
    QExchange* _curExchange = GetExchange(oExCode.ExchID);
    if(_curExchange)
    {
        tagXTNameTable oTable;
        int iret = _curExchange->GetOneNameTable(&oExCode, &oTable);
        if(iret >0)
        {
            strcpy(pname, oTable.Name);
            return 1;
        }
    }

    return -1;
}

std::vector<tagXTInstrument>& QDataModuleImpl::GetCodeList()
{
    return m_codeForAllExchanges;
}

void QDataModuleImpl::initCodeList()
{
    for(auto pExchange:m_arrExch)
    {
        int ncount = pExchange->GetCodeList(nullptr, 0);
        tagXTInstrument* pArr = new tagXTInstrument[ncount];
        ncount = pExchange->GetCodeList(pArr, ncount);
        for(int idx = 0; idx < ncount; ++idx)
        {
            m_codeForAllExchanges.push_back(pArr[idx]);
        }
        delete [] pArr;
    }
}
