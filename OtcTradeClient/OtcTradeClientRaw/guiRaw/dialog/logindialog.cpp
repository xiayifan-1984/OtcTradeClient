#include "logindialog.h"
#include "ui_logindialog.h"

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

#include <QDebug>

#include "../module/datamodule.h"
#include "../module/configmodule.h"
#include "../module/quotemodule.h"
#include "../module/trademodule.h"

#define     WM_TRADE_NOTIFY     (WM_USER+100)

//=============================================================================================================================================================================================================================================================
//=============================================================================================================================================================================================================================================================
//=============================================================================================================================================================================================================================================================
//=============================================================================================================================================================================================================================================================
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    m_nExecOK = 0;

    _timer = nullptr;

    m_nallTrade = 0;
    m_ncurTrade = 0;

    initControl();

    setAttribute(Qt::WA_DeleteOnClose);

}

LoginDialog::~LoginDialog()
{
    delete ui;
}


typedef		int			(*pfnReportStatus)(void* fthis, int main, int child);

#pragma pack(1)
typedef struct
{
    char				SaveDir[255];
    pfnReportStatus		Func;
    void*				fthis;
    char				_reserved[508];
}tagDataSetting;
#pragma pack()

void LoginDialog::initControl()
{



}

void    LoginDialog::initDataSubClient()
{
    m_dll.setFileName("StaticDataSubClient.dll");

    typedef int (__stdcall *pfnBeginWork)(tagDataSetting*);
    if(m_dll.load() )
    {
        pfnBeginWork p = (pfnBeginWork)m_dll.resolve("BeginWork");
        if(p)
        {
            tagDataSetting oinfo;

           strcpy(oinfo.SaveDir, GetConfigModule()->g.ExeDir );
           oinfo.fthis = this;
           oinfo.Func = s_ReportStatus;

           int iret = p(&oinfo);
           qDebug() << "StaticDataSubClient  beginwork" << iret;
        }
        else
        {
            qDebug() << "StaticDataSubClient  beginwork failed";
        }
    }
    else
    {
        qDebug() << "StaticDataSubClient dll load failed";
    }

}

int    LoginDialog::s_ReportStatus(void* fthis, int main, int child)
{
    LoginDialog* pthis = (LoginDialog*)fthis;
    pthis->onReportStatus(main, child);
}

void    LoginDialog::onReportStatus(int main, int child)
{
    //状态栏更新
    QString str;
    str.sprintf("main[%d] child[%d]", main, child);

    ui->labStatus->setText(str);

    if(main == 100 && child == 100)
    {
        m_nExecOK = 2;
        return;
    }

}

void	LoginDialog::accept()
{
    //do nothing
}
void	LoginDialog::reject()
{
    qDebug() << "LoginDialog::reject";

    int res = QMessageBox::question(nullptr, "Question", "Do you want to quit?");
    if(res == QMessageBox::Yes)
    {
        QDialog::reject();
    }
    else
    {
        //do nothing
    }

}

void LoginDialog::on_btnLogin_clicked()
{
    qDebug() << "on_btnLogin_clicked";

    //当登陆按钮点击,检查用户名和密码
    QString strUser = ui->editUser->text();
    QString strPwd = ui->editPwd->text();
    memset(GetConfigModule()->g.CurUser, 0, 255);
    memcpy(GetConfigModule()->g.CurUser, strUser.toLatin1().data(), strUser.size());
    if(strUser != "pengdk" && strUser != "xiayf")
    {
        QMessageBox::critical(nullptr, "Error", " Incorrect username or password");
        return;
    }

    ui->btnLogin->setEnabled(false);    //防止多次点击

    //启动定时器，检查是否完毕，完毕后调用QDialog::accept();
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    _timer->start(1000);

    m_nExecOK =1;
    initDataSubClient();

}

void    LoginDialog::onTimer()
{
    if(m_nExecOK == 2) //进入主界面
    {
        _timer->stop();

        m_nExecOK =3;

        //装在数据文件
        GetDataModule()->StartWork();

        int runmode = GetConfigModule()->g.RunMode;

        if(runmode == RUNMODE_NORMAL)
        {
            //启动行情模块
            GetQuoteModule()->StartWork();

            //补充交易账号对话框


            //配置模块重新载入
            GetConfigModule()->ReloadTradeUser();

            //初始化交易模块
            GetTradeModule()->Reload( GetConfigModule()->s.TradeLine, GetConfigModule()->s.TradeUser );

            //交易模块
            m_nallTrade = GetConfigModule()->s.TradeUserCount;
            HWND hWnd = (HWND)(this->winId() );
            GetTradeModule()->StartWork(hWnd, WM_TRADE_NOTIFY);

            QString str;
            str.sprintf("Trade StartWork");
            ui->labStatus->setText(str);
        }
        else if(runmode == RUNMODE_DATA2QUOTE)
        {
            //启动行情模块
            GetQuoteModule()->StartWork();
            //调用基类
            QDialog::accept();
        }
        else
        {
            //调用基类
            QDialog::accept();
        }
    }
}

void LoginDialog::closeEvent(QCloseEvent *event)
{
    typedef void (__stdcall *pfnEndWork)();

    if(m_dll.isLoaded())
    {
        pfnEndWork f1 = (pfnEndWork)m_dll.resolve("EndWork");
        if(f1)
        {
            f1();
        }
    }

    if(_timer)
    {
        delete _timer;
        _timer = nullptr;
    }

    event->accept();
}

//=============================================================================================================================================================================================================================================================
//=============================================================================================================================================================================================================================================================
//=============================================================================================================================================================================================================================================================
//=============================================================================================================================================================================================================================================================
bool     LoginDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    MSG* msg = reinterpret_cast<MSG*>(message);
    return winEvent(msg, result);
}


bool LoginDialog::winEvent(MSG *message, long *result)
{
    if(message->message == WM_TRADE_NOTIFY)
    {
        if (message->wParam == 100)
        {
            m_ncurTrade++;

            QString str;
            str.sprintf("Trade progress[%d][%d] current[%d][%d]", message->wParam, message->lParam, m_ncurTrade, m_nallTrade );
            ui->labStatus->setText(str);

            if (m_ncurTrade == m_nallTrade)
            {
                //调用基类
                QDialog::accept();
            }
        }
        else
        {
            QString str;
            str.sprintf("Trade progress[%d][%d] current[%d][%d]", message->wParam, message->lParam, m_ncurTrade, m_nallTrade );
            ui->labStatus->setText(str);
        }


    }

    return false;
}

