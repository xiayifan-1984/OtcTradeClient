#include "logindialog.h"

#include <QtCore>
#include <QtGui>

#include <QDebug>

#include "../module/datamodule.h"
#include "../module/configmodule.h"
#include "../module/quotemodule.h"
#include "../module/trademodule.h"

#include "parkordermgr.h"
#include "twapordermgr.h"

#define     WM_TRADE_NOTIFY     (WM_USER+100)

//=============================================================================================================================================================================================================================================================
//=============================================================================================================================================================================================================================================================
//=============================================================================================================================================================================================================================================================
//=============================================================================================================================================================================================================================================================
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent)
{
    _timer = nullptr;

    m_nExecOK = 0;
    m_nTradeCount = 0;
    m_nOkTradeNum = 0;

    m_nLastCheckID = 0;

    initControl();

    setAttribute(Qt::WA_DeleteOnClose);

}

LoginDialog::~LoginDialog()
{

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

    return 1;
}

void    LoginDialog::onReportStatus(int main, int child)
{
    //状态栏更新
    QString str;
    str.sprintf("main[%d] child[%d]", main, child);

    _lblStatus->setText(str);

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
    m_nLastCheckID = _gbtnView->checkedId();
    qDebug() << "LoginDialog::accept" << m_nLastCheckID;

    //当登陆按钮点击,检查用户名和密码
    QString strUser = _edtUser->text();
    QString strPwd = _edtPwd->text();
    memset(GetConfigModule()->g.CurUser, 0, 255);
    memcpy(GetConfigModule()->g.CurUser, strUser.toLatin1().data(), strUser.size());

    _btnLogin->setEnabled(false);   //防止多次点击

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

            //配置模块重新载入
            GetConfigModule()->ReloadTradeUser();

            vector<tagTTradeUserParam> arrUser;
            int numUsers = GetConfigModule()->GetAllTradeUser(arrUser);
            if(numUsers <= 0 )
            {
                QMessageBox::critical(nullptr, "Error", " Incorrect username or password");
                return;
            }

            //initParkOrder information
            GetParkedOrderMgr()->initUsers();
            GetTwapOrderMgr()->initUsers();

            //初始化交易模块
            GetTradeModule()->Reload( GetConfigModule()->s.TradeLine, GetConfigModule()->s.TradeUser );

            //交易模块
            m_nTradeCount = GetConfigModule()->s.TradeUserCount;
            HWND hWnd = (HWND)(this->winId() );
            GetTradeModule()->StartWork(hWnd, WM_TRADE_NOTIFY);

            QString str;
            str.sprintf("Trade StartWork");
            _lblStatus->setText(str);
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


bool LoginDialog::winEvent(MSG *message, long * /*result*/)
{
    if(message->message == WM_TRADE_NOTIFY)
    {
        if (message->wParam == 100)
        {
            m_nOkTradeNum++;

            QString str;
            str.sprintf("Trade progress[%d][%ld] current[%d][%d]", message->wParam, message->lParam, m_nOkTradeNum, m_nTradeCount );
            _lblStatus->setText(str);

            if (m_nOkTradeNum == m_nTradeCount)
            {
                //调用基类
                QDialog::accept();
            }
        }
        else
        {
            QString str;
            str.sprintf("Trade progress[%d][%ld] current[%d][%d]", message->wParam, message->lParam, m_nOkTradeNum, m_nTradeCount );
            _lblStatus->setText(str);
        }


    }

    return false;
}

void LoginDialog::initControl()
{
    _lblStatus = new QLabel();
    _btnLogin = new QPushButton(tr("登录"));
    _edtUser = new QLineEdit();
    _edtPwd = new QLineEdit();

    {
        _edtUser->setPlaceholderText(tr("用户名"));
        _edtPwd->setPlaceholderText(tr("密码"));

    }

    _rbtnSDI = new QRadioButton(tr("SDI(金牛)视图"));
    _rbtnMDI = new QRadioButton(tr("MDI视图"));

    //把几个Radio编成组，在组内互斥
    _gbtnView = new QButtonGroup();
    _gbtnView->addButton(_rbtnSDI, 0);
    _gbtnView->addButton(_rbtnMDI, 1);

    QHBoxLayout* h1 = new QHBoxLayout();
    h1->addWidget(_rbtnSDI);
    h1->addWidget(_rbtnMDI);

    QVBoxLayout* v1 = new QVBoxLayout();
    v1->addSpacing(50);
    v1->addWidget(_edtUser);
    v1->addWidget(_edtPwd);
    v1->addLayout(h1);
    v1->addWidget(_btnLogin);

    QHBoxLayout* h2 = new QHBoxLayout();
    h2->addSpacing(50);
    h2->addLayout(v1);
    h2->addSpacing(240);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(h2);
    mainLayout->addWidget(_lblStatus);

    this->setLayout(mainLayout);

    //[2]设置图片
    m_oBkImg.load(":/image/login.png");

    //[3]设置object Name
    this->setProperty("name","logindialog");

    //[4]设定窗口为固定大小
    helper_fixedsize();

    //[5]设置标题
    this->setWindowTitle(tr("登录"));

    //[6]绑定消息
    QObject::connect(_btnLogin, SIGNAL(clicked()), this, SLOT(on_btnLogin_clicked()) );

    //[7]设定默认值
    _rbtnSDI->setChecked(true);
}

void    LoginDialog::helper_fixedsize()
{
    int width = 500;
    int height = 211;
    this->resize(width, height);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->setMinimumSize(QSize(width, height));
    this->setMaximumSize(QSize(width, height));
    this->setSizeGripEnabled(false);
}

int    LoginDialog::GetViewMode()
{
    qDebug() << "GetViewMode " << m_nLastCheckID;
    return  m_nLastCheckID;
}

void LoginDialog::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(rect(), m_oBkImg);
    painter.end();
}




