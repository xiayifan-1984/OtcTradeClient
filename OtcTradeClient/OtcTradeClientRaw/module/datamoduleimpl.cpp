#include "datamoduleimpl.h"
#include <string.h>
#include <algorithm>


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
    initNameList();
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

int QDataModuleImpl::GetAllCodeList(std::vector<tagXTInstrument>& arr)
{
    arr.insert( arr.end(), m_codeForAllExchanges.begin(), m_codeForAllExchanges.end() );
    return m_codeForAllExchanges.size();
}

int  QDataModuleImpl::GetAllNameTableList(std::vector<tagXTNameTable>& arr)
{
    arr.insert( arr.end(), m_nameForAllExchanges.begin(), m_nameForAllExchanges.end() );
    return m_nameForAllExchanges.size();
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

void QDataModuleImpl::initNameList()
{
    for(auto pExchange:m_arrExch)
    {
        int ncount = pExchange->GetNameTableList(nullptr, 0);
        tagXTNameTable* pArr = new tagXTNameTable[ncount];
        ncount = pExchange->GetNameTableList(pArr, ncount);
        for(int idx = 0; idx < ncount; ++idx)
        {
            m_nameForAllExchanges.push_back(pArr[idx]);
        }
        delete [] pArr;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QExchangeImpl::QExchangeImpl()
{
    m_pArrData = 0;
    m_nDataSize = 0;
}

int         QExchangeImpl::Init(int id)
{
    m_nExchID = id;
    return 1;
}
void        QExchangeImpl::Free()
{
    if (m_pArrData)
    {
        delete[] m_pArrData;
        m_pArrData = nullptr;
    }

    m_nDataSize = 0;
    m_mapCode2Index.clear();
    m_arrProduct.clear();
}

int         QExchangeImpl::GetExchID()
{
    return m_nExchID;
}

int         QExchangeImpl::GetExchName(char* pbuf)
{
    switch(m_nExchID)
    {
    case XT_EXCH_CFFEX:
        strcpy(pbuf, XT_EXCHNAME_CFFEX);
        break;
    case XT_EXCH_INE:
        strcpy(pbuf, XT_EXCHNAME_INE);
        break;
    case XT_EXCH_SHFE:
        strcpy(pbuf, XT_EXCHNAME_SHFE);
        break;
    case XT_EXCH_DCE:
        strcpy(pbuf, XT_EXCHNAME_DCE);
        break;
    case XT_EXCH_CZCE:
        strcpy(pbuf, XT_EXCHNAME_CZCE);
        break;
    }

    return 1;
}

int         QExchangeImpl::GetExchCHName(char* pbuf)
{
    switch(m_nExchID)
    {
    case XT_EXCH_CFFEX:
        strcpy(pbuf, XT_EXCHCHNNAME_CFFEX);
        break;
    case XT_EXCH_INE:
        strcpy(pbuf, XT_EXCHCHNNAME_INE);
        break;
    case XT_EXCH_SHFE:
        strcpy(pbuf, XT_EXCHCHNNAME_SHFE);
        break;
    case XT_EXCH_DCE:
        strcpy(pbuf, XT_EXCHCHNNAME_DCE);
        break;
    case XT_EXCH_CZCE:
        strcpy(pbuf, XT_EXCHCHNNAME_CZCE);
        break;
    }

    return 1;
}

bool        QExchangeImpl::IsExistCode(tagXTInstrument& inout)
{
    int nlen = strlen(inout.Code);
    char szcode[32]={0};
    strcpy(szcode, inout.Code);

    string strkey = szcode;
    transform(strkey.begin(),strkey.end(),strkey.begin(),::toupper);
    map<string, int>::iterator itb = m_mapCode2Index.find(strkey);
    if(itb != m_mapCode2Index.end() )
    {
        strncpy(inout.Code, m_pArrData[itb->second].ExCode.Code, nlen);
        return true;
    }

    return false;
}

int         QExchangeImpl::GetCodeList(tagXTInstrument* pArr, int nsize)
{
    if (nullptr == pArr)
    {
        return m_nDataSize;
    }

    int nmin = m_nDataSize;
    if (nsize < nmin)
    {
        nmin = nsize;
    }

    for (int i = 0; i < nmin; i++)
    {
        pArr[i] = m_pArrData[i].ExCode;
    }

    return nmin;
}

int         QExchangeImpl::GetNameTableList(tagXTNameTable* pArr, int nsize)
{
    if (nullptr == pArr)
    {
        return m_nDataSize;
    }

    int nmin = m_nDataSize;
    if (nsize < nmin)
    {
        nmin = nsize;
    }

    memcpy(pArr, m_pArrData, sizeof(tagXTNameTable)* nmin);

    return nmin;
}

int         QExchangeImpl::GetOneNameTable(tagXTInstrument* p, tagXTNameTable* po)
{
    string strkey = p->Code;
    transform(strkey.begin(),strkey.end(),strkey.begin(),::toupper);
    map<string, int>::iterator itb = m_mapCode2Index.find(strkey);
    if (itb != m_mapCode2Index.end())
    {
        *po = m_pArrData[itb->second];
        return 1;
    }

    return -2;
}

int         QExchangeImpl::GetProductList(tagXTProduct* pArr, int nsize)
{
    int count = m_arrProduct.size();
    if (nullptr == pArr)
    {
        return count;
    }

    int nmin = count;
    if (nsize < nmin)
    {
        nmin = nsize;
    }

    for (int i = 0; i < nmin; i++)
    {
        pArr[i] = m_arrProduct[i];
    }
    return nmin;
}

int         QExchangeImpl::GetCodeListByProduct(tagXTInstrument* pArr, int nsize, int pid)
{
    int cur = 0;
    int total = 0;
    for (int i = 0; i < m_nDataSize; i++)
    {
        if (m_pArrData[i].InnerProductID == pid)
        {
            total++;
            if (pArr && cur < nsize)
            {
                pArr[cur] = m_pArrData[i].ExCode;
                cur++;
            }
        }
    }

    if (nullptr == pArr)
    {
        return total;
    }
    else
    {
        return cur;
    }
}

int         QExchangeImpl::GetNameListByProduct(tagXTNameTable* pArr, int nsize, int pid)
{
    int cur = 0;
    int total = 0;
    for (int i = 0; i < m_nDataSize; i++)
    {
        if (m_pArrData[i].InnerProductID == pid)
        {
            total++;
            if (pArr && cur < nsize)
            {
                pArr[cur] = m_pArrData[i];
                cur++;
            }
        }
    }

    if (nullptr == pArr)
    {
        return total;
    }
    else
    {
        return cur;
    }
}

int         QExchangeImpl::GetCodeListByProductID(tagXTInstrument* pArr, int nsize, const char* pid)
{
    int innerid = inner_getProductID(pid);
    if (innerid >= 0)
    {
        return GetCodeListByProduct(pArr, nsize, innerid);
    }

    return -1;
}

int         QExchangeImpl::inner_getProductID(const char* pid)
{
    int count = m_arrProduct.size();
    for (int i = 0; i < count; i++)
    {
        if (stricmp(m_arrProduct[i].ProductID, pid) == 0)
        {
            return m_arrProduct[i].InnerProductID;
        }
    }

    return (-1);
}

void        QExchangeImpl::inner_readProduct(const char* pDataFile)
{
    FILE* pf = fopen(pDataFile, "r");
    if (0 == pf)
    {
        return;
    }

    int innerID =0;
    while (1)
    {
        if (feof(pf) || ferror(pf))
        {
            break;
        }

        char szline[1024] = { 0 };
        fgets(szline, 1023, pf);

        int nlen = strlen(szline);

        tagXTProduct oItem;
        memset(&oItem, 0, sizeof(oItem));
        if (inner_parseOneProduct(szline, nlen, oItem))
        {
            oItem.InnerProductID = innerID;
            m_arrProduct.push_back(oItem);
            innerID++;
        }
    }

    fclose(pf);
}

void        QExchangeImpl::inner_readCode(const char* pDataFile)
{
    vector<tagXTNameTable>  arrStock;

    FILE* pf = fopen(pDataFile, "r");
    if (0 == pf)
    {
        return;
    }

    while (1)
    {
        if (feof(pf) || ferror(pf))
        {
            break;
        }

        char szline[1024] = { 0 };
        fgets(szline, 1023, pf);

        int nlen = strlen(szline);

        tagXTNameTable oItem;
        memset(&oItem, 0, sizeof(oItem));
        if (inner_parseOneItem(szline, nlen, oItem))
        {
            arrStock.push_back(oItem);
        }
    }

    fclose(pf);

    m_nDataSize = arrStock.size();
    m_pArrData = new tagXTNameTable[m_nDataSize + 1];
    for (int i = 0; i < m_nDataSize; i++)
    {
        m_pArrData[i] = arrStock[i];
        string strkey = m_pArrData[i].ExCode.Code;

        //先把所有代码变为大写，作为索引
        transform(strkey.begin(),strkey.end(),strkey.begin(),::toupper);
        m_mapCode2Index.insert(make_pair(strkey, i));
        m_pArrData[i].InnerProductID = inner_getProductID(m_pArrData[i].ProductID);
    }
}

int         QExchangeImpl::ReadFile(const char* pszDataDir, const char* pszCode, const char* pszProduct)
{
    char szProductFile[255] = { 0 };
    strcpy(szProductFile, pszDataDir);
    strcat(szProductFile, pszProduct);
    inner_readProduct(szProductFile);

    char szCodeFile[255] = { 0 };
    strcpy(szCodeFile, pszDataDir);
    strcat(szCodeFile, pszCode);
    inner_readCode(szCodeFile);

    return 1;
}

bool		QExchangeImpl::inner_parseOneProduct(char* pszbuf, int buflen, tagXTProduct& oItem)
{
    if (buflen < 20)
    {
        return false;
    }

    int npos = 0;
    char seps[] = ",";
    char* context = 0;
    char * token = strtok_s(pszbuf, seps, &context);
    while (token != NULL)
    {
        switch (npos)
        {
        case 0: //代码
        {
            strcpy(oItem.ProductID, token);
        }
            break;
        case 1:  //名称
        {
            strcpy(oItem.Name, token);
        }
            break;
        case 2: //交易所
        {
            if (_strnicmp(token, "CFFEX", 5) == 0)
            {
                oItem.ExchID = XT_EXCH_CFFEX;
            }
            else if (_strnicmp(token, "SHFE", 4) == 0)
            {
                oItem.ExchID = XT_EXCH_SHFE;
            }
            else if (_strnicmp(token, "CZCE", 4) == 0)
            {
                oItem.ExchID = XT_EXCH_CZCE;
            }
            else if (_strnicmp(token, "DCE", 4) == 0)
            {
                oItem.ExchID = XT_EXCH_DCE;
            }
            else if (_strnicmp(token, "INE", 4) == 0)
            {
                oItem.ExchID = XT_EXCH_INE;
            }
            else
            {
                return false;
            }
        }
            break;
        case 3:  // 产品类型
        {
            oItem.ProductClass = atoi(token);
        }
            break;
        case 4:
        {
            oItem.Multiple = atoi(token);
        }
            break;
        case 5:
        {
            double d = atof(token);
            oItem.PriceTick = d;
        }
            break;

        }

        npos++;
        token = strtok_s(NULL, seps, &context);

    }

    return true;
}

bool        QExchangeImpl::inner_parseOneItem(char* pszbuf, int buflen, tagXTNameTable& oItem)
{
    if (buflen < 20)
    {
        return false;
    }

    int deliveryyear = 0;
    int deliverymonth = 0;

    int npos = 0;
    char seps[] = ",";
    char* context = 0;
    char * token = strtok_s(pszbuf, seps, &context);
    while (token != NULL)
    {
        switch (npos)
        {
        case 0:
        {
            strcpy(oItem.ExCode.Code, token);
        }
            break;
        case 1:
        {
            if (_strnicmp(token, "CFFEX", 5) == 0)
            {
                oItem.ExCode.ExchID = XT_EXCH_CFFEX;
            }
            else if (_strnicmp(token, "SHFE", 4) == 0)
            {
                oItem.ExCode.ExchID = XT_EXCH_SHFE;
            }
            else if (_strnicmp(token, "CZCE", 4) == 0)
            {
                oItem.ExCode.ExchID = XT_EXCH_CZCE;
            }
            else if (_strnicmp(token, "DCE", 4) == 0)
            {
                oItem.ExCode.ExchID = XT_EXCH_DCE;
            }
            else if (_strnicmp(token, "INE", 4) == 0)
            {
                oItem.ExCode.ExchID = XT_EXCH_INE;
            }
            else
            {
                return false;
            }
        }
            break;
        case 2:
            strcpy(oItem.Name, token);
            break;
        case 3:
            strcpy(oItem.ContractID, token);
            break;
        case 4:
            strcpy(oItem.ProductID, token);
            break;
        case 5:
            oItem.ProductClass = atoi(token);
            break;
        case 6:
            deliveryyear = atoi(token);
            break;
        case 7:
        {
            deliverymonth = atoi(token);
            oItem.Delivery = deliveryyear * 100 + deliverymonth;
        }
            break;
        case 8:
            oItem.VolumeMultiple = atoi(token);
            break;
        case 9:
            oItem.PriceTick = atof(token);
            break;
        case 10:
            //创建日
            break;
        case 11:
            //上市日
            break;
        case 12:	//到期日
            oItem.ExpireDate = atoi(token);
            break;
        case 13: //开始交割日
        case 14: //结束交割日
        case 15: //合约周期状态
        case 16: //是否交易
        case 17: //持仓类型
        case 18: //持仓日期类型
        case 19: //标的代码
            break;
        case 20: //执行价
        {
            double d = atof(token);
            oItem.ExecPrice = SHOPT_NPRICE(d);
        }
            break;
        }

        npos++;
        token = strtok_s(NULL, seps, &context);
    }

    //Group
    //oItem.Group = XT_GROUP_FUTURE;

    //2019-3-26 add
    oItem.LotSize =1;

    //确定showdot    2019-3-26 add
    if(oItem.PriceTick < 1.0 )
    {
        int nvalue = oItem.PriceTick * 100000;
        int bits =0;
        while(nvalue >= 10)
        {
            nvalue = nvalue/10;
            bits++;
        }

        oItem.ShowDot = 5 - bits;
    }
    else
    {
        oItem.ShowDot = 0;
    }

    return true;
}















