
#ifndef SDIQUOTEFRAME_DEFINE_H
#define SDIQUOTEFRAME_DEFINE_H

#include <QtWidgets>

class QOrderMgr;
class CDockTitlebar;
class ModelSingleQuote;
class CSDIQuoteFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CSDIQuoteFrame(QWidget *parent = nullptr);
    virtual ~CSDIQuoteFrame();

public:
    void                setUserAccount(int usertype, int broker, char* userid);
    void                setUserAccount(QOrderMgr* p);
    void                initControls();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                onTimer();

private:
    QTableView*         _tblViewNormal;
    ModelSingleQuote*   _modelNormal;

    QOrderMgr*          _orderMrg;

private:
    CDockTitlebar*      _titlebar;
    QTimer*             _timer;

};




#endif








