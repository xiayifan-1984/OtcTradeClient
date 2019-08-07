#include <windows.h>
#include <dbghelp.h>

#include <QtCore>
#include <QApplication>

#include "logindialog.h"
#include "mainwindow.h"

#include "./module/configmodule.h"
#include "./module/datamodule.h"
#include "./module/quotemodule.h"
#include "./module/trademodule.h"
#include "./module/internalmessaging.h"
#include "./util/codeinnermsg.h"
#include "./util/XTCodec.h"
#include "./util/stool.h"
#include <string.h>
#include "stool.h"

void requestOtcPostions()
{
    auto loginName = stool::loginName();
    auto innerOtcInquiry = Codeinnermsg::otcOptInquiryReq("1", 1, loginName.c_str());
    //Sleep(10000);
    int msgCount = 0;
    while(msgCount < 3)
    {
        Sleep(1000);
        ++msgCount;
        GetInternalMsgSenderReceiver()->sendMsg("410", const_cast<char*>(innerOtcInquiry.c_str()), innerOtcInquiry.size());
    }
}

//=========================================================================================================================================================================================================================================

long ApplicationCrashHandler(EXCEPTION_POINTERS *pException){
{
    // 在程序exe的上级目录中创建dmp文件夹
    QDir *dmp = new QDir;
    bool exist = dmp->exists("../dmp/");
    if(exist == false)
        dmp->mkdir("../dmp/");
    }

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy_MM_dd_hh_mm_ss");
    QString time =  current_date + ".dmp";

    EXCEPTION_RECORD *record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode, 16));
    QString errAddr(QString::number((uint)record->ExceptionAddress, 16));
    QString errFlag(QString::number(record->ExceptionFlags, 16));
    QString errPara(QString::number(record->NumberParameters, 16));
    qDebug()<<"errCode: "<<errCode;
    qDebug()<<"errAddr: "<<errAddr;
    qDebug()<<"errFlag: "<<errFlag;
    qDebug()<<"errPara: "<<errPara;

    HANDLE hDumpFile = CreateFile((LPCWSTR)QString("../dmp/" + time).utf16(),
        GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hDumpFile != INVALID_HANDLE_VALUE) {
          MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
          dumpInfo.ExceptionPointers = pException;
          dumpInfo.ThreadId = GetCurrentThreadId();
          dumpInfo.ClientPointers = TRUE;
          MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
          CloseHandle(hDumpFile);
     }
     else{
          qDebug()<<"hDumpFile == null";
     }
     return EXCEPTION_EXECUTE_HANDLER;
}

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
		}
	}
}

void    OnInitInstance()
{
    GetConfigModule()->Init();

    GetDataModule()->Init( GetConfigModule()->g.ShareDataDir );

    GetInternalMsgSenderReceiver()->instantiateSenderReceiver();
    GetInternalMsgSenderReceiver()->initProducer(GetConfigModule()->g.kafkaServer);
    char brokers[255]{0};
    strncpy(brokers, GetConfigModule()->g.kafkaServer, strlen(GetConfigModule()->g.kafkaServer));

    char* topics[] =
    {
        "411",
        "421"
    };
    auto groupid = stool::uniqueGroupId("kafka");
    GetInternalMsgSenderReceiver()->initConsumer(brokers, groupid.c_str(), internalMsgHandler,2,topics);

    GetInternalMsgSenderReceiver()->startConsume();

 //   GetInternalMsgSenderReceiver()->sendMsg("410", const_cast<char*>(innerOtcInquiry.c_str()), innerOtcInquiry.size());
 //   GetInternalMsgSenderReceiver()->sendMsg("410", const_cast<char*>(innerOtcInquiry.c_str()), innerOtcInquiry.size());

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

    GetInternalMsgSenderReceiver()->stopConsume();
    Sleep(1000);
    GetInternalMsgSenderReceiver()->Release();
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

int main(int argc, char *argv[])
{
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
	QApplication a(argc, argv);

    qApp->setWindowIcon(QIcon(":/image/main.ico"));

    OnInitInstance();

    initStyle();

    MainWindow* pMainWnd = nullptr;

    LoginDialog* pDlg = new LoginDialog();
    int rec = pDlg->exec();
    if (rec == QDialog::Accepted)
    {
        pMainWnd = new MainWindow();
        pMainWnd->showMaximized();
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
