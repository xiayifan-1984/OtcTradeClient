#ifndef OTCRISKCALC_H
#define OTCRISKCALC_H

#include "XTBase.h"
#include <QtWidgets>
#include <vector>
#include <memory>
#include <utility>
#include "otcoptionmodule.h"

class OtcRiskCalc : public QWidget
{
    Q_OBJECT

public:
    explicit OtcRiskCalc(QWidget *parent = nullptr);
    ~OtcRiskCalc(){};

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


protected slots:
    void                onWidget_itemDoubleClicked(QTableWidgetItem*);
   // void                ontimer();
protected:
    QTimer*             m_pTimer;
private:
    QTableWidget*       _tblWidget;
    tagXTInstrument     m_curCode;
    double              m_vol;
    int                 m_position;
    FutureOptData       m_otcOptData;

public:
    void                setInstrument(tagXTInstrument&);

private:
    std::vector<double> m_riskCombs;
    void updateRiskComb(double price, int row, int col);
    void updateRiskAfterNewParaInput();
};


#endif // OTCRISKCALC_H
