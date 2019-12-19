
#ifndef VIEWINQUIRYBOX_DEFINE_H
#define VIEWINQUIRYBOX_DEFINE_H

#include <QtWidgets>
#include "./util/decodeinnermsg.h"
#include "otcoptionmodule.h"

class ViewInquiryBox : public QWidget
{
    Q_OBJECT

public:
    explicit ViewInquiryBox(QWidget *parent = nullptr);
    ~ViewInquiryBox();

protected:
    void                initControls();
    void                setWidgetData();

protected slots:
    void                onWidget_itemDoubleClicked(QTableWidgetItem*);

protected:
    //void              closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    void                initRow(int rowno);
    void                fillRow(int rowno, const otcOptPosition& opt);

protected slots:
    void                onTimer();
private:
    QTableWidget*       _tblWidget;
    FutureOptData       m_otcOptFuture;
    QTimer*             m_pTimer;
    std::vector<otcOptPosition> m_otcPositions;
};

#endif
