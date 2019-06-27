
#include "trademoduleimpl.h"
#include "ordermgrimpl.h"
//=================================================================================================================================================================================================================================================================================================

#pragma pack(1)

typedef struct
{
    char			tradedll[255];
}tagXTTradeDll;

typedef struct
{
    bool			needauth;
    char			frontaddr[255];
}tagXTLineSetting;

typedef struct
{
    int				userType;
    int				broker;
    char			user[64];
    char			psw[64];
    char			userproductinfo[64];
    char			authcode[64];
}tagXTUserSetting;

typedef struct
{
    tagXTLineSetting		line;
    tagXTUserSetting		user;
}tagXTCreateTradeParam;

#pragma pack()

//=================================================================================================================================================================================================================================================================================================
QTradeModuleImpl::QTradeModuleImpl()
{
    m_hWnd =0;
    m_nMsg =0;
}

int         QTradeModuleImpl::Init()
{
    m_mapUser.clear();
    m_mapDll.clear();
    m_arrLines.clear();
    m_arrUsers.clear();

    return 1;
}

void        QTradeModuleImpl::Free()
{
    m_arrLines.clear();
    m_arrUsers.clear();
}

int         QTradeModuleImpl::Reload(const char* plinefile, const char* puserfile)
{
    m_arrLines.clear();
    m_arrUsers.clear();

    char szinifile[255]={0};
    strcpy(szinifile, plinefile);
    loadLineFile(szinifile);

    memset(szinifile, 0, sizeof(szinifile));
    strcpy(szinifile, puserfile);
    loadUserFile(szinifile);

    m_mapUser.clear();
    m_mapDll.clear();

    return 1;
}

QOrderMgr*  QTradeModuleImpl::GetMgr(int type, int broker, const char* pszuser)
{
    char szbuf[255]={0};
    _snprintf(szbuf, 254, "%d_%d_%s", broker, type, pszuser);

    string strkey = szbuf;
    map<string, QOrderMgrImpl*>::iterator itb = m_mapUser.find(strkey);
    if (itb != m_mapUser.end())
    {
        return itb->second;
    }

    return 0;
}

void        QTradeModuleImpl::loadLineFile(const char* plinefile)
{
    int ncount = GetPrivateProfileIntA("main", "linecount", 0, plinefile);
    for(int i=0; i< ncount; i++)
    {
        tagTradeLine oinfo;
        memset(&oinfo, 0, sizeof(oinfo));

        char szkey[32]={0};
        _snprintf(szkey, 31, "line_%d", i);
        oinfo.id = GetPrivateProfileIntA(szkey, "id", 0, plinefile);

        GetPrivateProfileStringA(szkey, "dll", "", oinfo.dll, 254, plinefile);

        GetPrivateProfileStringA(szkey, "addr", "", oinfo.front, 254, plinefile);
        GetPrivateProfileStringA(szkey, "trade_dll","", oinfo.tradedll, 254, plinefile);

        char szbuf[32]={0};
        GetPrivateProfileStringA(szkey, "needauth", "", szbuf, 31, plinefile);
        if(strnicmp(szbuf, "true", 4) == 0)
        {
            oinfo.needauth = true;
        }
        else
        {
            oinfo.needauth = false;
        }

        m_arrLines.push_back(oinfo);
    }
}

void        QTradeModuleImpl::loadUserFile(const char* puserfile)
{
    int ncount = GetPrivateProfileIntA("main", "usercount", 0, puserfile);
    for(int i=0; i< ncount; i++)
    {
        tagTradeUser oinfo;
        memset(&oinfo, 0, sizeof(oinfo));

        char szkey[32]={0};
        _snprintf(szkey, 31, "user_%d", i);
        oinfo.type = GetPrivateProfileIntA(szkey, "type", 0, puserfile);
        oinfo.broker = GetPrivateProfileIntA(szkey, "broker", 0, puserfile);
        GetPrivateProfileStringA(szkey, "user", "", oinfo.user, 32, puserfile);
        GetPrivateProfileStringA(szkey, "psw", "", oinfo.psw, 32, puserfile);
        GetPrivateProfileStringA(szkey, "userproductinfo", "", oinfo.userproductinfo, 32, puserfile);
        GetPrivateProfileStringA(szkey, "authcode", "", oinfo.authcode, 32, puserfile);

        oinfo.lineno = GetPrivateProfileIntA(szkey, "lineno", 0, puserfile);

        m_arrUsers.push_back(oinfo);
    }

}

void        QTradeModuleImpl::initDllLoader()
{
    int ncount = m_arrLines.size();
    for(int i=0; i<ncount; i++)
    {
        int lineno = m_arrLines[i].id;

        HMODULE hDll = LoadLibraryA(m_arrLines[i].dll);
        if(0 == hDll)
        {
            continue;
        }

        typedef int (__stdcall *pfnBeginWork)(tagXTTradeDll* p);
        pfnBeginWork pfn = (pfnBeginWork)::GetProcAddress(hDll, "BeginWork");
        if(pfn)
        {
            tagXTTradeDll oinfo;
            memset(&oinfo, 0, sizeof(oinfo));
            strcpy(oinfo.tradedll, m_arrLines[i].tradedll);
            pfn(&oinfo);
        }
        else
        {
            continue;
        }
        m_mapDll.insert( make_pair( lineno, hDll) );
    }

}

void        QTradeModuleImpl::initOneUser()
{
    int ncount = m_arrUsers.size();
    for(int i=0; i<ncount; i++)
    {
        HMODULE hDll = findDllFromLine(m_arrUsers[i].lineno);
        if(0 == hDll)
        {
            continue;
        }

        tagTradeLine* pLine = nullptr;
        for(int j=0; j < m_arrLines.size(); j++)
        {
            if(m_arrLines[j].id == m_arrUsers[i].lineno)
            {
                pLine = &(m_arrLines[j]);
                break;
            }
        }
        if(nullptr == pLine)
        {
            continue;
        }

        typedef CXTTradeApi*	(__stdcall *pfnCreateTradeApi)(tagXTCreateTradeParam* , char* );
        pfnCreateTradeApi pfn = (pfnCreateTradeApi)::GetProcAddress(hDll, "CreateTradeApi");
        if(pfn)
        {
            tagXTCreateTradeParam oParam;
            memset(&oParam, 0, sizeof(oParam));
            oParam.user.userType = m_arrUsers[i].type;
            oParam.user.broker = m_arrUsers[i].broker;
            strcpy(oParam.user.user, m_arrUsers[i].user);
            strcpy(oParam.user.psw, m_arrUsers[i].psw);
            strcpy(oParam.user.userproductinfo, m_arrUsers[i].userproductinfo);
            strcpy(oParam.user.authcode, m_arrUsers[i].authcode);

            strcpy(oParam.line.frontaddr, pLine->front);
            oParam.line.needauth = pLine->needauth;

            char szerr[256]={0};
            CXTTradeApi* pApi = pfn(&oParam, szerr);

            QOrderMgrImpl* pMgr = new QOrderMgrImpl(hDll , pApi);
            pMgr->Instance(m_hWnd, m_nMsg, m_arrUsers[i].broker, m_arrUsers[i].type, m_arrUsers[i].user);

            char szkey[128]={0};
            _snprintf(szkey, 127, "%d_%d_%s", m_arrUsers[i].broker, m_arrUsers[i].type, m_arrUsers[i].user );
            std::string strkey = szkey;
            m_mapUser.insert(make_pair( strkey  ,pMgr) );
        }

    }

}

void        QTradeModuleImpl::SetHWnd(HWND hWnd, unsigned int nMsg)
{
    m_hWnd = hWnd;
    m_nMsg = nMsg;

    map<string, QOrderMgrImpl*>::iterator itb = m_mapUser.begin();
    while(itb != m_mapUser.end() )
    {
        itb->second->setHWnd(m_hWnd, m_nMsg);
        itb++;
    }
}

int         QTradeModuleImpl::StartWork(HWND hWnd, unsigned int nMsg)
{
    m_hWnd = hWnd;
    m_nMsg = nMsg;

    initDllLoader();
    initOneUser();

    return 1;
}

void        QTradeModuleImpl::EndWork()
{
    map<string, QOrderMgrImpl*>::iterator itb = m_mapUser.begin();
    while (itb != m_mapUser.end() )
    {
        QOrderMgrImpl* pImpl = itb->second;
        pImpl->Release();

        delete pImpl;

        itb++;
    }

    m_mapUser.clear();

    map<int, HMODULE>::iterator it = m_mapDll.begin();
    while(it != m_mapDll.end() )
    {
        HMODULE hDll  = it->second;
        if(hDll)
        {
            typedef void    (__stdcall *pfnEndWork)();
            pfnEndWork pfn = (pfnEndWork)::GetProcAddress(hDll, "EndWork");
            if(pfn)
            {
                pfn();
            }

            FreeLibrary(hDll);
        }

        it++;
    }
    m_mapDll.clear();

}

HMODULE     QTradeModuleImpl::findDllFromLine(int lineno)
{
    map<int, HMODULE>::iterator it = m_mapDll.find(lineno);
    if(it != m_mapDll.end() )
    {
        return it->second;
    }

    return 0;
}


