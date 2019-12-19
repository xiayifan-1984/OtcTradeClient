#ifndef DATAMODULEIMPLDEFINE_H
#define DATAMODULEIMPLDEFINE_H

#include "datamodule.h"
#include <vector>
#include <map>
using namespace std;


class QDataModuleImpl :public QDataModule
{
public:
    QDataModuleImpl();

public:
    int         	Init(const char* pszDataDir);
    void        	Free();

    int         	StartWork();
    void        	EndWork();

    int         	GetExchangeList(std::vector<QExchange*>& arr);
    int         	VerifyExCode(tagXTInstrument& inout);
    QExchange*  	GetExchange(int exchid);
    int         	GetOneName(tagXTInstrument& oExCode, char* pname);
    int             GetAllCodeList(std::vector<tagXTInstrument>& arr);
    int             GetAllNameTableList(std::vector<tagXTNameTable>& arr);

protected:
    void        	initCodeList();
    void            initNameList();

protected:
    std::vector<QExchange*>  	m_arrExch;
    char        				m_szDataDir[255];

    std::vector<tagXTInstrument> m_codeForAllExchanges;
    std::vector<tagXTNameTable>  m_nameForAllExchanges;

};



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

    bool	inner_parseOneProduct(char* pszbuf, int buflen, tagXTProduct& oItem);
    bool        inner_parseOneItem(char* pszbuf, int buflen, tagXTNameTable& oItem);
protected:
    int         m_nExchID;

    map<string, int>        m_mapCode2Index;
    tagXTNameTable*         m_pArrData;
    int                     m_nDataSize;

    vector<tagXTProduct>    m_arrProduct;

};



#endif
