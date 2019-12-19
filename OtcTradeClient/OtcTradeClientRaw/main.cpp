
#include <windows.h>

#include <QtCore>
#include <QApplication>

#include "logindialog.h"
#include "mainwindow.h"

#include "./module/configmodule.h"
#include "./module/datamodule.h"
#include "./module/quotemodule.h"
#include "./module/trademodule.h"
#include "./module/kafkamsgunit.h"
#include "./util/codeinnermsg.h"
#include "./util/XTCodec.h"
#include "./util/stool.h"
#include <string.h>
#include "stool.h"
#include "parkordermgr.h"
#include "codeinnermsg.h"

#include "sdiwindow.h"

//=========================================================================================================================================================================================================================================
//=========================================================================================================================================================================================================================================
//=========================================================================================================================================================================================================================================

void    initStyle()
{
    QString fileName = ":/qss/silvery.css";
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly))
        {
            QString str = file.readAll();
            qApp->setStyleSheet(str);
            file.close();
        }
    }
}

void    initKafaModule()
{
    GetKafkaUnit()->Instance();
    GetKafkaUnit()->initProducer(GetConfigModule()->s.kafkaServer);
    char brokers[255]{0};
    strncpy(brokers, GetConfigModule()->s.kafkaServer, strlen(GetConfigModule()->s.kafkaServer));

    char *topics[3] =
    {
        "411",
        "423",
        "431",
    };

    auto groupid = stool::uniqueGroupId("kafka");
    GetKafkaUnit()->initConsumer(brokers, groupid.c_str(), fnMsgHandler,3, topics);

    GetKafkaUnit()->startConsume();
}

void    freeKafkaModule()
{
    GetKafkaUnit()->stopConsume();
    Sleep(1000);
    GetKafkaUnit()->Release();
}

//=========================================================================================================================================================================================================================================
//=========================================================================================================================================================================================================================================
//=========================================================================================================================================================================================================================================
void    OnInitInstance()
{
    GetConfigModule()->Init();

    GetDataModule()->Init( GetConfigModule()->g.ShareDataDir );

    initKafaModule();

    int runmode = GetConfigModule()->g.RunMode;
    if(runmode == RUNMODE_NORMAL)
    {
        GetQuoteModule()->Init();

        GetTradeModule()->Init();
    }
    else if(runmode == RUNMODE_DATA2QUOTE)
    {
        GetQuoteModule()->Init();
    }
    else
    {
        //only data//do nothing
    }
}

void    OnEndInstance()
{
    int runmode = GetConfigModule()->g.RunMode;
    if(runmode == RUNMODE_NORMAL)
    {
        GetTradeModule()->EndWork();

        GetQuoteModule()->EndWork();
    }
    else if(runmode == RUNMODE_DATA2QUOTE)
    {
        GetQuoteModule()->EndWork();
    }

    GetDataModule()->EndWork();
}

void    OnExitInstance()
{
    int runmode = GetConfigModule()->g.RunMode;
    if(runmode == RUNMODE_NORMAL)
    {
        GetTradeModule()->Free();

        GetQuoteModule()->Free();
    }
    else if(runmode == RUNMODE_DATA2QUOTE)
    {
        GetQuoteModule()->Free();
    }

    GetDataModule()->Free();

    GetConfigModule()->Free();

    freeKafkaModule();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //[1]设置程序图标
    qApp->setWindowIcon(QIcon(":/image/main.ico"));

    //[2]设置全局字体，防止QT 自己觉得分辨率变化，然后自己把字体变大
    QFont font = qApp->font();
    font.setPointSize(9);
    qApp->setFont(font);

    //[3]
    OnInitInstance();

    initStyle();

    LoginDialog* pDlg = new LoginDialog();
    int rec = pDlg->exec();
    if (rec == QDialog::Accepted)
    {
        if(pDlg->GetViewMode() == 1)
        {
            //显示MDI
            CMainWindow* pMainWnd = nullptr;
            pMainWnd = new CMainWindow();
            pMainWnd->showMaximized();
        }
        else
        {
            //显示SDI
            CSDIWindow* pSdiWnd = new CSDIWindow();
            if(GetConfigModule()->g.RunMode == RUNMODE_NORMAL)
            {
                std::vector<tagTTradeUserParam> arrUser;
                GetConfigModule()->GetAllTradeUser(arrUser);
                tagTTradeUserParam& o = arrUser[0];
                QString strName;
                std::string strmsg = o.aliasuser;
                strName = XTCodec::AfGbk_ToQString(strmsg);
                QOrderMgr* p = GetTradeModule()->GetMgr(o.type, o.broker, o.user);
                QVariant v = QVariant::fromValue((void *)p);

                pSdiWnd->setSDIUser(v, strName);
            }

            pSdiWnd->initControls();
            pSdiWnd->showMaximized();
        }

    }
    else
    {
        qDebug() << "rec" << rec;
        return 0;
    }

    int res= a.exec();

    OnEndInstance();

    OnExitInstance();

    return  res;
}
