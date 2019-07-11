#ifndef DATAMODULE_H
#define DATAMODULE_H

#include "XTStaticData.h"
#include <vector>
//=====================================================================================================================================================================================================================

class QExchange
{
public:
	virtual ~QExchange();
protected:
	QExchange();

public:
	virtual     int         GetExchID() = 0;
    virtual     int         GetExchName(char* pbuf) = 0;
    virtual     int         GetExchCHName(char* pbuf) = 0;
    virtual     bool        IsExistCode(tagXTInstrument& inout) = 0;
	virtual     int         GetCodeList(tagXTInstrument* pArr, int nsize) = 0;
	virtual     int         GetNameTableList(tagXTNameTable* pArr, int nsize) = 0;
	virtual     int         GetOneNameTable(tagXTInstrument* p, tagXTNameTable* po) = 0;

public:
	virtual     int         GetProductList(tagXTProduct* pArr, int nsize) = 0;
	virtual     int         GetCodeListByProduct(tagXTInstrument* pArr, int nsize, int pid) = 0;
	virtual     int         GetCodeListByProductID(tagXTInstrument* pArr, int nsize, const char* pid) = 0;
    virtual     int         GetNameListByProduct(tagXTNameTable* pArr, int nsize, int pid) = 0;

};

class QDataModule
{
public:
	virtual ~QDataModule();
protected:
	QDataModule();

public:
	virtual     int         Init(const char* pszDataDir) = 0;
	virtual     void        Free() = 0;

	virtual     int         StartWork() = 0;
	virtual     void        EndWork() = 0;

	virtual     int         GetExchangeList(std::vector<QExchange*>& arr) = 0;
    virtual     int         VerifyExCode(tagXTInstrument& inout) = 0;
    virtual     QExchange*  GetExchange(int exchid) = 0;
    virtual     int         GetOneName(tagXTInstrument& oExCode, char* pname) =0;
    virtual     std::vector<tagXTInstrument>& GetCodeList() = 0;
};


extern  QDataModule*        GetDataModule();



#endif // DATAMODULE_H
