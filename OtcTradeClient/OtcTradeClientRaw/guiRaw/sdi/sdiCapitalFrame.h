#ifndef SDICAPITALFRAME_DEFINE_H
#define SDICAPITALFRAME_DEFINE_H

#include <QtWidgets>

class QOrderMgr;
class CDockTitlebar;
class ModelSingleCapital;
class CSDICapitalFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CSDICapitalFrame(QWidget *parent = nullptr);
    virtual ~CSDICapitalFrame();

public:
    void                setUserAccount(int usertype, int broker, char* userid);
    void                setUserAccount(QOrderMgr* p);
    void                initControls();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                onQueryBtnClick();
    void                onTimer();

private:
    QTableView*         _tblViewNormal;
    ModelSingleCapital* _modelNormal;

    QPushButton*        _btnQuery;
    QOrderMgr*          _orderMrg;

private:
    CDockTitlebar*      _titlebar;
    QTimer*             _timer;

};




#endif
