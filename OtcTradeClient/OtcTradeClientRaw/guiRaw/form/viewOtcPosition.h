
#ifndef VIEWOTCPOSITION_DEFINE_H
#define VIEWOTCPOSITION_DEFINE_H

#include <QtWidgets>
#include "XTBase.h"
#include "./module/otcoptionmodule.h"
#include "./optpricingapplication/futureoptdata.h"

class ViewOtcPosition : public QWidget
{
    Q_OBJECT

public:
    explicit ViewOtcPosition(QWidget *parent = nullptr);
    ~ViewOtcPosition();

public:
    void                setCurCommodity(tagXTInstrument&  oExCode);
    void                setCurPrice(double p);

protected:
    void                initControls();
    void                setWidgetData();

protected:
    //void              closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    void                initRow(int rowno);
    void                initTotal(int rowno, double avoidSize, double timeValue, double totalProfit);
    void                fillRow(int rowno, const otcOptPosition& opt, double& avoidSize, double& timeValue, double& totalProfit);
protected slots:
    void                onTimer();

private:
    QTableWidget*       _tblWidget;
    tagXTInstrument     _curExCode;
    std::vector<std::string> m_optPositions;
    QTimer*              m_pTimer;
    double               m_curPrice;
    FutureOptData        m_otcOptFuture;
};

#endif
