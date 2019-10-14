
#include "configmodule.h"
#include <QtCore>
#include <windows.h>
#include "XTCodec.h"
#include "XTCodec.h"
#include <string.h>
#include "./optpricingapplication/futureoptdata.h"
#include <QTime>

//=================================================================================================================================================================================================================================
QConfigModule*      g_configModule = nullptr;

QConfigModule*      GetConfigModule()
{
    if(nullptr == g_configModule)
    {
        g_configModule = new QConfigModule();
    }

    return g_configModule;
}

QConfigModule::QConfigModule()
{

}

int     QConfigModule::Init()
{
    memset(&s, 0, sizeof (tagSessionSetting));
    memset(&g, 0, sizeof(tagGlobalSetting));
    memset(&m, 0, sizeof(tagMemorySave));

    QString st= qApp->applicationDirPath();
    std::string sts = st.toStdString();

    //[1]
    strcpy(g.ExeDir, sts.c_str() );
    strcat(g.ExeDir, "/");

    //[2]
    strcpy(g.ShareDataDir, g.ExeDir);
    strcat(g.ShareDataDir, "sharedata/");

    //[3]
    strcpy(g.UserDir, g.ExeDir);
    strcat(g.UserDir, "user/");

    //[4]
    initClient();
    initMainContract();
    initTradeTimeSection();
    initVolatilitys();

    return 1;
}

void    QConfigModule::Free()
{

}

void    QConfigModule::ReloadTradeUser()
{
    m_arrTradeUser.clear();
    memset(&s, 0, sizeof (tagSessionSetting));

    initTradeUser();
}

void    QConfigModule::initTradeUser()
{
    //[1]
    strcpy(s.TradeLine, g.UserDir);
    strcat(s.TradeLine, g.CurUser);
    strcat(s.TradeLine, "/");
    strcat(s.TradeLine, "TradeLine.ini");

    //[2]
    strcpy(s.TradeUser, g.UserDir);
    strcat(s.TradeUser, g.CurUser);
    strcat(s.TradeUser, "/");
    strcat(s.TradeUser, "TradeUser.ini");

    //[3]
    s.TradeUserCount = GetPrivateProfileIntA("main", "usercount", 0, s.TradeUser);

    //[4]
    for(int i=0; i< s.TradeUserCount; i++)
    {
        char szkey[32]={0};
        _snprintf(szkey, 31, "user_%d", i);

        tagOneTradeUser oinfo;
        memset(&oinfo, 0, sizeof(oinfo));
        oinfo.type = GetPrivateProfileIntA(szkey, "type", 0, s.TradeUser);
        oinfo.broker = GetPrivateProfileIntA(szkey, "broker", 0, s.TradeUser);
        GetPrivateProfileStringA(szkey, "user", "", oinfo.user, 31, s.TradeUser );
        GetPrivateProfileStringA(szkey, "aliasuser", "", oinfo.aliasuser, 63, s.TradeUser);
        GetPrivateProfileStringA(szkey, "tradelimit", "", oinfo.tradelimit, 511, s.TradeUser);

        m_arrTradeUser.push_back(oinfo);
    }

}

void    QConfigModule::initClient()
{
    //读取 cfg/client.json
    char szfile[255]={0};
    strcpy(szfile, g.ExeDir);
    strcat(szfile, "cfg/client.json");

    QFile loadFile(szfile);

    if(!loadFile.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {

        return;
    }

    QJsonObject rootObj = jsonDoc.object();

    //因为是预先定义好的JSON数据格式，所以这里可以这样读取
    g.RunMode = RUNMODE_NORMAL;
    if(rootObj.contains("mode"))
    {
        QString str = rootObj.value("mode").toString();
        if(str == "1")
        {
            g.RunMode = RUNMODE_ONLYDATA;
        }
        else if(str == "2")
        {
            g.RunMode = RUNMODE_DATA2QUOTE;
        }
    }

    if(rootObj.contains("volatility"))
    {
        QString str = rootObj.value("volatility").toString();
        g.vol = str.toDouble();
        qDebug() << "start vol = " << g.vol;
    }

    if(rootObj.contains("riskfreerate"))
    {
        QString str = rootObj.value("riskfreerate").toString();
        g.riskFreeRate = str.toDouble();
        qDebug() << "risk free rate = " << g.riskFreeRate;
    }

    if(rootObj.contains("kafka"))
    {
        QString str = rootObj.value("kafka").toString();

        memset(g.kafkaServer, '\0', 254);
        strncpy(g.kafkaServer, str.toLatin1().data(), str.size());
        qDebug() << "kafka server = " << g.kafkaServer;
    }

    // read holidays
    char holidayfile[255]={0};
    strcpy(holidayfile, g.ExeDir);
    strcat(holidayfile, "cfg/holidays.json");

    QFile hFile(holidayfile);
    if(!hFile.open(QIODevice::ReadOnly))
    {
        return;
    }

    allData = hFile.readAll();
    hFile.close();

    QJsonDocument hjsonDoc(QJsonDocument::fromJson(allData, &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }

    QJsonObject holidayObj = hjsonDoc.object();
    if(holidayObj.contains("holidays"))
    {
        QJsonArray subObj = holidayObj.value("holidays").toArray();
        for(auto p = subObj.begin(); p != subObj.end(); ++p)
        {
            FutureOptData::addHoliday(p->toString());
        }
    }

}

int     QConfigModule::GetAllTradeUser(vector<tagOneTradeUser>& arrUser)
{
    arrUser.assign(m_arrTradeUser.begin(), m_arrTradeUser.end() );

    return arrUser.size();
}

int     QConfigModule::GetTradeUserBy(const char* pProductID, vector<tagOneTradeUser>& arrUser)
{
    int productlen = strlen(pProductID);

    int nsize = m_arrTradeUser.size();
    for(int i=0; i< nsize; i++)
    {
        int nlen = strlen(m_arrTradeUser[i].tradelimit);
        if( nlen <= 0)
        {
            continue;
        }

        qDebug() << "tradelimit" << m_arrTradeUser[i].tradelimit << " target product " << pProductID;
        char* p = m_arrTradeUser[i].tradelimit;
        char* begin = p;
        for(int j=0; j<nlen; j++)
        {
            if(*p == ',' || j == nlen-1)
            {
               // qDebug() << "product len " << productlen << " compared len " << p-begin;
                if(productlen == (p - begin) )
                {
                    if(_strnicmp(begin, pProductID, productlen) == 0)
                    {
                        arrUser.push_back(m_arrTradeUser[i]);
                        break;
                    }
                }

                begin = p+1;
            }

            p++;
        }

    }

    return arrUser.size();
}

void    QConfigModule::initMainContract()
{
    //读取 sharedata/main_contract.json
    char szfile[255]={0};
    strcpy(szfile, g.ShareDataDir);
    strcat(szfile, "main_contract.json");

    QFile loadFile(szfile);
    if(!loadFile.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }

    QJsonObject rootObj = jsonDoc.object();

    //按交易所读取
    if(rootObj.contains("CZCE"))
    {
        QJsonObject subObj = rootObj.value("CZCE").toObject();
        QStringList keys = subObj.keys();
        for(int i = 0; i < keys.size(); i++)
        {
            tagXTInstrument  obj;
            memset(&obj, 0, sizeof(obj));
            obj.ExchID = XT_EXCH_CZCE;
            std::string strcode = XTCodec::AfUtf8_ToString( keys.at(i) );
            strcpy( obj.Code, strcode.c_str() );

            m_arrMainContract.push_back(obj);
        }

    }
    if(rootObj.contains("SHFE"))
    {
        QJsonObject subObj = rootObj.value("SHFE").toObject();
        QStringList keys = subObj.keys();
        for(int i = 0; i < keys.size(); i++)
        {
            tagXTInstrument  obj;
            memset(&obj, 0, sizeof(obj));
            obj.ExchID = XT_EXCH_SHFE;
            std::string strcode = XTCodec::AfUtf8_ToString( keys.at(i) );
            strcpy( obj.Code, strcode.c_str() );

            m_arrMainContract.push_back(obj);
        }

    }
    if(rootObj.contains("DCE"))
    {
        QJsonObject subObj = rootObj.value("DCE").toObject();
        QStringList keys = subObj.keys();
        for(int i = 0; i < keys.size(); i++)
        {
            tagXTInstrument  obj;
            memset(&obj, 0, sizeof(obj));
            obj.ExchID = XT_EXCH_DCE;
            std::string strcode = XTCodec::AfUtf8_ToString( keys.at(i) );
            strcpy( obj.Code, strcode.c_str() );

            m_arrMainContract.push_back(obj);
        }

    }
    if(rootObj.contains("INE"))
    {
        QJsonObject subObj = rootObj.value("INE").toObject();
        QStringList keys = subObj.keys();
        for(int i = 0; i < keys.size(); i++)
        {
            tagXTInstrument  obj;
            memset(&obj, 0, sizeof(obj));
            obj.ExchID = XT_EXCH_INE;
            std::string strcode = XTCodec::AfUtf8_ToString( keys.at(i) );
            strcpy( obj.Code, strcode.c_str() );

            m_arrMainContract.push_back(obj);
        }

    }

}

void QConfigModule::initTradeTimeSection()
{
    char secfile[255]={0};
    strcpy(secfile, g.ShareDataDir);
    strcat(secfile, "trade_timesection.json");

    QFile secFile(secfile);
    if(!secFile.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray sec_allData = secFile.readAll();
    secFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonSecDoc(QJsonDocument::fromJson(sec_allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }

    QJsonObject rootSecObj = jsonSecDoc.object();
    QStringList keys = rootSecObj.keys();
    for(int i = 0; i < keys.size(); ++i)
    {
        const auto& key = XTCodec::AfUtf8_ToString( keys.at(i));
        QJsonArray subArray = rootSecObj.value(keys.at(i)).toArray();
        m_tradeTimeSection[key].resize(subArray.size()/2);
        for(int j = 0; j<subArray.size(); j+=2)
        {
            if(j<subArray.size() && j+1 < subArray.size())
            {
                m_tradeTimeSection[key][j/2].first = QTime::fromString(subArray[j].toString(), "HH:mm:ss");
                m_tradeTimeSection[key][j/2].second = QTime::fromString(subArray[j+1].toString(), "HH:mm:ss");
            }
        }
    }
}

void QConfigModule::initVolatilitys()
{
    char secfile[255]={0};
    strcpy(secfile, g.ShareDataDir);
    strcat(secfile, "volatility.json");

    QFile secFile(secfile);
    if(!secFile.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray allData = secFile.readAll();
    secFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        return;
    }

    QJsonObject rootObj = jsonDoc.object();
    if(rootObj.contains("volatility"))
    {
        QJsonArray volList = rootObj.value("volatility").toArray();
        for(auto pEle = volList.begin(); pEle != volList.end(); ++pEle)
        {
            QJsonObject subObj = pEle->toObject();
            auto keys = subObj.keys();
            auto value = subObj.value(keys[0]).toDouble();

            std::string inst = XTCodec::AfUtf8_ToString(keys[0]);
            //auto search = m_instRiskVols.find(inst);
            m_instRiskVols[inst] = value;
            qDebug() << "instrument name = " << keys[0] << " value = " << m_instRiskVols[inst];
        }
    }
}

void QConfigModule::storeVolatilitys()
{
    char secfile[255]={0};
    strcpy(secfile, g.ShareDataDir);
    strcat(secfile, "volatility.json");

    QFile secFile(secfile);
    if(!secFile.open(QIODevice::ReadWrite))
    {
        return;
    }
    secFile.resize(0);
    QJsonArray contents;
    int i = 0;
    for(auto p = m_instRiskVols.begin(); p != m_instRiskVols.end(); ++p)
    {
        QJsonObject ele;
        ele.insert(p->first.c_str(), p->second);
        contents.insert(i, ele);
        ++i;
    }
    QJsonObject outer;
    outer.insert("volatility", contents);
    QJsonDocument json;
    json.setObject(outer);

    secFile.write(json.toJson());
    secFile.close();
}

int       QConfigModule::GetAllMainContract(vector<tagXTInstrument>& arrExCode)
{
    arrExCode.assign(m_arrMainContract.begin(), m_arrMainContract.end() );

    return arrExCode.size();
}

void     QConfigModule::GetOrderRef(char* pszOrderRef,  char* pszMarker)
{
    QTime odt = QTime::currentTime();
    int ncurtime = odt.hour()*10000 + odt.minute()*100 + odt.second();
    if(ncurtime != m.lastReqTime)
    {
        m.lastReqTime = ncurtime;
        m.reqInsertNo =1;
    }

    _snprintf(pszOrderRef, 6, "%06d", ncurtime);
    _snprintf(pszOrderRef+6, 3, "%03d", ++m.reqInsertNo%1000 );

    memcpy(pszOrderRef+9, pszMarker, 3);
}

void QConfigModule::insertRiskVol(tagXTInstrument &code, double vol)
{
    std::string inst(code.Code);
    auto search = m_instRiskVols.find(inst);
    if(search != m_instRiskVols.end())
    {
        m_instRiskVols[inst] = vol;
    }
    else
    {
        m_instRiskVols.insert({inst, vol});
    }
    storeVolatilitys();
}

double QConfigModule::getRiskVol(tagXTInstrument &code)
{
    std::string inst(code.Code);
    auto search = m_instRiskVols.find(inst);
    if(search != m_instRiskVols.end())
    {
        return m_instRiskVols[inst];
    }
    return 0.2;
}

const TimeSections &QConfigModule::getMarketOpenedTimeSec()
{
    return m_tradeTimeSection;
}





