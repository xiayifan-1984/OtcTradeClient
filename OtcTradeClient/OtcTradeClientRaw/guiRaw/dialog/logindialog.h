#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QtWidgets>
#include <QLibrary>


class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    virtual ~LoginDialog();

public:
    int                 GetViewMode();

protected:
    void                initControl();
    void                initDataSubClient();

    virtual void		accept();
    virtual void		reject();
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    bool                nativeEvent(const QByteArray &eventType, void *message, long *result);
    bool                winEvent(MSG *message, long *result);

    void                paintEvent(QPaintEvent *event);

protected:
    static      int     s_ReportStatus(void* fthis, int main, int child);
    void                onReportStatus(int main, int child);

private slots:
    void                on_btnLogin_clicked();
    void                onTimer();

private:
    void                helper_fixedsize();

protected:
    QLineEdit*          _edtUser;
    QLineEdit*          _edtPwd;
    QPushButton*        _btnLogin;
    QLabel*             _lblStatus;

    QRadioButton*       _rbtnSDI;
    QRadioButton*       _rbtnMDI;
    QButtonGroup*       _gbtnView;

private:
    QTimer*             _timer;
    QLibrary            m_dll;
    QPixmap             m_oBkImg;

    int                 m_nExecOK;          //状态
    int                 m_nTradeCount;    //交易账号的个数
    int                 m_nOkTradeNum;        //当前的已经OK的交易账号的个数

private:
    int                 m_nLastCheckID;

};



#endif // LOGINDIALOG_H
