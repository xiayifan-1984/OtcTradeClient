#ifndef QUOTEMODULEDEFINE_H
#define QUOTEMODULEDEFINE_H

#include "XTQuoteStruct.h"

class QQuoteModule
{
public:
    virtual ~QQuoteModule();
protected:
    QQuoteModule();

public:
    virtual		int		Init() = 0;

    virtual		void	Free() = 0;

    virtual     int     StartWork() = 0;
    virtual     void    EndWork() = 0;

    virtual     void    SetCodeList(tagXTInstrument* pArr, int nCount) = 0;


    virtual		int		GetSnapShot(tagXTInstrument& ExCode, tagXTSnapshot* pin) = 0;

    virtual		int		GetSnapShot2(int exchange, const char* code, tagXTSnapshot* pin) = 0;

};


extern  QQuoteModule*        GetQuoteModule();

extern double GetCurrentPrice(tagXTInstrument&);

#endif
