#ifndef EXCHANGEIMPLDEFINE_H
#define EXCHANGEIMPLDEFINE_H
#include "datamodule.h"

#include <vector>
#include <string>
#include <map>
using namespace std;

class QExchangeImpl : public QExchange
{
public:
	QExchangeImpl();

public:
	int         Init(int id);
	void        Free();
	int         ReadFile(const char* pszDataDir, const char* pszCode, const char* pszProduct);

public:
	int         GetExchID();
    int         GetExchName(char* pbuf);
    int         GetExchCHName(char* pbuf);
    bool        IsExistCode(tagXTInstrument& inout);
	int         GetCodeList(tagXTInstrument* pArr, int nsize);
	int         GetNameTableList(tagXTNameTable* pArr, int nsize);
	int         GetOneNameTable(tagXTInstrument* p, tagXTNameTable* po);

public:
	int         GetProductList(tagXTProduct* pArr, int nsize);
	int         GetCodeListByProduct(tagXTInstrument* pArr, int nsize, int pid);
	int         GetCodeListByProductID(tagXTInstrument* pArr, int nsize, const char* pid);
    int         GetNameListByProduct(tagXTNameTable* pArr, int nsize, int pid);

protected:
	int         inner_getProductID(const char* pid);
	void        inner_readProduct(const char* pDataFile);
	void        inner_readCode(const char* pDataFile);

	bool		inner_parseOneProduct(char* pszbuf, int buflen, tagXTProduct& oItem);
	bool        inner_parseOneItem(char* pszbuf, int buflen, tagXTNameTable& oItem);
protected:
	int         m_nExchID;

	map<string, int>        m_mapCode2Index;
	tagXTNameTable*         m_pArrData;
	int                     m_nDataSize;

	vector<tagXTProduct>    m_arrProduct;

};


#endif
