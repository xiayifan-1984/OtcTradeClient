
#include "quotemoduleimpl.h"

#include <string>
#include <map>
#include <mutex>
#include "parkorderbox.h"
#include "ordermgrimpl.h"
#include <vector>
extern ParkOrderBox* getParkOrderBox();
using namespace std;

#include <QDebug>

extern QOrderMgr* GetOrderMgrbyInstrument(tagXTInstrument& curCode);
//====================================================================================================================================================================================================
class	CtpMarket
{
public:
    CtpMarket()
    {
        m_pArrData =nullptr;
        m_nDataSize =0;
    }

    virtual ~CtpMarket()
    {

    }

public:
    int			Instance()
    {
        Release();

        m_nDataSize = 500;
        m_pArrData = new tagXTSnapshot[m_nDataSize];

        return 1;
    }

    void		Release()
    {
        if (m_pArrData)
        {
            delete []m_pArrData;
            m_pArrData =nullptr;
        }
        m_nDataSize =0;

        m_mapCode2Shot.clear();
    }

    int			GetSnapShot(int exchange, const char* pcode, tagXTSnapshot* pin)
    {
        string strkey = pcode;

        map<string, int>::iterator itb = m_mapCode2Shot.find(strkey);
        if (itb != m_mapCode2Shot.end() )
        {
            *pin = m_pArrData[ itb->second ];
            pin->ExCode.ExchID = exchange;		//这里，反向赋值回去

            return 1;
        }

        return 0;
    }

    void		OnRecvData( tagXTSnapshot* pIn)
    {
        std::lock_guard<std::mutex> lck (mtx);

        string strkey = pIn->ExCode.Code;

        tagXTSnapshot oCopy;
        memcpy(&oCopy, pIn, sizeof(oCopy) );
        //normalize_snapshot(oCopy);

        map<string, int>::iterator itb = m_mapCode2Shot.find(strkey);
        if (itb != m_mapCode2Shot.end() )
        {
            m_pArrData[itb->second] = oCopy;
        }
        else
        {
            int no = getnext();

            m_pArrData[no] = oCopy;

            m_mapCode2Shot.insert(make_pair(strkey, no));
        }
    }

protected:
    int			getnext()
    {
        int n1 = m_mapCode2Shot.size();

        int nextno = n1;

        if (nextno +5 > m_nDataSize)
        {
            int		oldsize = m_nDataSize;
            tagXTSnapshot* pold = m_pArrData;

            m_nDataSize += 200;
            m_pArrData = new tagXTSnapshot[m_nDataSize];
            memcpy(m_pArrData, pold, sizeof(tagXTSnapshot)*oldsize );

            delete []pold;
        }

        return nextno;
    }

protected:
    tagXTSnapshot*                  m_pArrData;
    int								m_nDataSize;
    map<string, int>				m_mapCode2Shot;

    std::mutex                      mtx;

};

//====================================================================================================================================================================================================
//====================================================================================================================================================================================================
//====================================================================================================================================================================================================
//====================================================================================================================================================================================================
QQuoteModuleImpl::QQuoteModuleImpl()
{
    m_pCtpMarket = nullptr;
}

int		QQuoteModuleImpl::GetSnapShot(tagXTInstrument& ExCode, tagXTSnapshot* pin)
{
    return GetSnapShot2(ExCode.ExchID, ExCode.Code, pin);
}

int		QQuoteModuleImpl::GetSnapShot2(int exchange, const char* code, tagXTSnapshot* pin)
{
    if(m_pCtpMarket)
    {
        return m_pCtpMarket->GetSnapShot(exchange, code, pin);
    }
    return 0;
}

typedef		void(*pfnQuoteRecvData)(void* fthis, int Market, int MsgType, const char * pszBuf, int nBytes);

#pragma pack(1)
typedef struct
{
    pfnQuoteRecvData		Func;				//回调函数
    void*					fthis;				//
    char					_reserved[256];
}tagQuoteSetting;

#pragma pack()


int     QQuoteModuleImpl::StartWork()
{
    typedef int (__stdcall *pfnBeginWork)(tagQuoteSetting*);
    m_dll.setFileName("TransrcCtp6311Raw.dll");

    if(m_dll.load())
    {
        pfnBeginWork f1 = (pfnBeginWork)m_dll.resolve("BeginWork");
        if(f1)
        {
            tagQuoteSetting oinfo;
            memset(&oinfo, 0, sizeof(oinfo));
            oinfo.fthis = this;
            oinfo.Func = s_OnRecvData;

            return f1(&oinfo);
        }

        return (-2);
    }
    return (-1);
}

void    QQuoteModuleImpl::EndWork()
{
    typedef  void (__stdcall *pfnEndWork)();

    if(m_dll.isLoaded())
    {
        pfnEndWork f1 = (pfnEndWork)m_dll.resolve("EndWork");
        if(f1)
        {
            f1();
        }
    }

}

void    QQuoteModuleImpl::SetCodeList(tagXTInstrument* pArr, int nCount)
{
    qDebug() << "SetCodeList";

    vector<tagXTInstrument>  subList;

    for(int i=0; i<nCount; i++)
    {
        string strcode = pArr[i].Code;
        if(m_arrCode.find( strcode) == m_arrCode.end())
        {
            subList.push_back(pArr[i]);

            m_arrCode.insert(strcode);
        }
    }

    int nsize = subList.size();
    if(nsize <=0)
    {
        return;
    }

    typedef  void (__stdcall *pfnSetCodeList)(tagXTInstrument*, int);

    if(m_dll.isLoaded() )
    {
        pfnSetCodeList f1 = (pfnSetCodeList)m_dll.resolve("SetCodeList");
        if(f1)
        {
            f1( &*subList.begin(), nsize );

            qDebug() << "SetCodeList nsize =" << nsize;
        }
    }
}

int		QQuoteModuleImpl::Init()
{
    m_pCtpMarket = new CtpMarket();
    m_pCtpMarket->Instance();

    return 1;
}

void	QQuoteModuleImpl::Free()
{
    if(m_pCtpMarket)
    {
        m_pCtpMarket->Release();
        delete m_pCtpMarket;
        m_pCtpMarket =nullptr;
    }

}

void    QQuoteModuleImpl::s_OnRecvData(void* fthis, int Market, int MsgType, const char * pszBuf, int nBytes)
{
    QQuoteModuleImpl* pthis = (QQuoteModuleImpl*)fthis;
    if(pthis)
    {
        pthis->OnRecvData(Market, MsgType, pszBuf, nBytes);
    }
}

void    QQuoteModuleImpl:: OnRecvData(int Market, int MsgType, const char * pszBuf, int nBytes)
{
    Q_UNUSED(Market)
    Q_UNUSED(MsgType)
    Q_UNUSED(nBytes)
    tagXTSnapshot* pIn = (tagXTSnapshot*)pszBuf;
    if(m_pCtpMarket)
    {
        m_pCtpMarket->OnRecvData(pIn);
    }
}


