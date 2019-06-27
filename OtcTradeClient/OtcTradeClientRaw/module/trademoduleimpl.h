#ifndef TRADEMODULEIMPL_DEFINE_H
#define TRADEMODULEIMPL_DEFINE_H

#include "trademodule.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

#include <windows.h>

class QOrderMgrImpl;

class QTradeModuleImpl : public QTradeModule
{
public:
    QTradeModuleImpl();

public:
    int         Init();
    void        Free();

    int         Reload(const char* plinefile, const char* puserfile);
    int         StartWork(HWND hWnd, unsigned int nMsg);
    void        EndWork();
    void        SetHWnd(HWND hWnd, unsigned int nMsg);

    QOrderMgr*  GetMgr(int type, int broker, const char* pszuser);

protected:
    void        loadLineFile(const char* plinefile);
    void        loadUserFile(const char* puserfile);
    void        initDllLoader();
    void        initOneUser();
    HMODULE     findDllFromLine(int lineno);

protected:
    struct tagTradeLine
    {
        int         id;
        char        dll[255];

        char        front[255];
        char        tradedll[255];
        bool        needauth;
    };
    std::vector<tagTradeLine>       m_arrLines;

    struct  tagTradeUser
    {
        int         type;
        int         broker;
        char        user[64];
        char        psw[64];
        char        userproductinfo[64];
        char        authcode[64];
        int         lineno;
    };
    vector<tagTradeUser>       m_arrUsers;

    map<string, QOrderMgrImpl*>   m_mapUser;      //string=broker_utype_user, QOrderMgrImpl
    map<int, HMODULE>          m_mapDll;       //lineno ===>dll

protected:
    HWND            m_hWnd;
    unsigned int    m_nMsg;

public:
    vector<tagTradeUser>& getAllUsers()
    {
        return m_arrUsers;
    }

};


#endif
