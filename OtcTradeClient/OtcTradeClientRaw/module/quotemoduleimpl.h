#ifndef QUOTEMODULEIMPL_DEFINE_H
#define QUOTEMODULEIMPL_DEFINE_H
#include "quotemodule.h"
#include <QLibrary>
#include <string>
#include <set>
using namespace std;

class CtpMarket;
class QQuoteModuleImpl :public QQuoteModule
{
public:
    QQuoteModuleImpl();

public:
    int		Init();

    void	Free();

    int     StartWork();
    void    EndWork();

    void    SetCodeList(tagXTInstrument* pArr, int nCount);

    int		GetSnapShot(tagXTInstrument& ExCode, tagXTSnapshot* pin);

    int		GetSnapShot2(int exchange, const char* code, tagXTSnapshot* pin);

protected:
    static  void    s_OnRecvData(void* fthis, int Market, int MsgType, const char * pszBuf, int nBytes);
    void            OnRecvData(int Market, int MsgType, const char * pszBuf, int nBytes);

protected:
    CtpMarket*      m_pCtpMarket;
    QLibrary        m_dll;
    set<string>     m_arrCode;      //注册Code

};







#endif
