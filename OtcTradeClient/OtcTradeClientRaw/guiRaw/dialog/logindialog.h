#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLibrary>

namespace Ui {
class LoginDialog;
}

class Titlebar;
class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

protected:
    void                initControl();
    void                initDataSubClient();

    virtual void		accept();
    virtual void		reject();
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    bool                nativeEvent(const QByteArray &eventType, void *message, long *result);
    bool                winEvent(MSG *message, long *result);

protected:
    static      int     s_ReportStatus(void* fthis, int main, int child);
    void                onReportStatus(int main, int child);

private slots:
    void                on_btnLogin_clicked();
    void                onTimer();

private:
    QTimer*             _timer;
    QLibrary            m_dll;

    int                 m_nExecOK;      //状态
    int                 m_nallTrade;
    int                 m_ncurTrade;

    Ui::LoginDialog *   ui;
};

/*
 *
 *
 *
 *
 */


#endif // LOGINDIALOG_H
