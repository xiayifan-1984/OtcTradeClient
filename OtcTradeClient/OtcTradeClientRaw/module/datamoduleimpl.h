#ifndef DATAMODULEIMPLDEFINE_H
#define DATAMODULEIMPLDEFINE_H

#include "datamodule.h"
#include <vector>
class QDataModuleImpl :public QDataModule
{
public:
	QDataModuleImpl();

public:
	int         Init(const char* pszDataDir);
	void        Free();

	int         StartWork();
	void        EndWork();

	int         GetExchangeList(std::vector<QExchange*>& arr);
    int         VerifyExCode(tagXTInstrument& inout);
    QExchange*  GetExchange(int exchid);
    int         GetOneName(tagXTInstrument& oExCode, char* pname);

protected:
	std::vector<QExchange*>  m_arrExch;
	char        m_szDataDir[255];

};

#endif
