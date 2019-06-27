#ifndef VIEWRISKAVOID_DEFINE_H
#define VIEWRISKAVOID_DEFINE_H

#include "XTBase.h"
#include <QtWidgets>
#include <vector>
#include "ordermgrimpl.h"
#include "kbinputbox.h"
#include <memory>
#include <utility>
#include "./optpricingapplication/futureoptdata.h"

class ViewRiskAvoid : public QWidget
{
    Q_OBJECT

public:
    explicit ViewRiskAvoid(QWidget *parent = nullptr);
    ~ViewRiskAvoid();

protected:
    void                initControls();
    void                setWidgetData();

protected:
    //void              closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void                initRow001();
    void                initRow002();
    void                initRow003();
    void                initRow004();
    void                initRow005();

    void                updateRow001(int, double);
    void                updateRow002_003_004_005(double);
    QOrderMgr*          getOrderMgrbyInstrument();


protected slots:
    void                onWidget_itemDoubleClicked(QTableWidgetItem*);
    void                ontimer();

protected:
    QTimer*             m_pTimer;
private:
    QTableWidget*       _tblWidget;
    tagXTInstrument     m_curCode;
    double              m_vol;
    int                 m_position;
    std::weak_ptr<KBInputBox>         m_tradeBox;
    int                 m_widgetPosition;

    //std::weak_ptr<KBInputBox>         getSingleTradeBox();
    void                init8WidgetsPostions();
    FutureOptData       m_otcOptData;

public:
    void                setInstrument(tagXTInstrument&);
    std::pair<long, long>   getRecommendPostion();
};

#endif
