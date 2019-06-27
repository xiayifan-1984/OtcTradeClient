#ifndef FUTUREOPTDATA_H
#define FUTUREOPTDATA_H

#include <set>
#include <QtCore>
#include "./util/decodeinnermsg.h"
#include "./greek/futureoptpricing.h"

class FutureOptData
{
public:
    FutureOptData();
    ~FutureOptData() = default;
    static void addHoliday(QString str);
    void setRiskVol(double vol);
    void setRiskFreeRate(double rate);
    void update(double price, otcOptPosition& opt);
    static double calcCurElapseTime(QDate endDate);
    double getRiskAmount();
    double getOneGamma();
    double getDelta();
    double getBsPrice();
    double getTheta();

private:
    double           calcAvoidRiskAmount(const otcOptPosition& opt) const;
    double           calcOneVolGamma(const otcOptPosition& opt) const;
    double           calcDelta(const otcOptPosition& opt) const;
    double           calcBsPrice(const otcOptPosition& opt) const;
    double           calcTheta(const otcOptPosition& opt) const;
    static std::set<QDate>  m_holidays;
    double           m_riskVol;
    double           m_riskFreeRate;
    double           m_avoidRiskAmount;
    double           m_gammaForOneVol;
    double           m_elapseTime;
    double           m_curPrice;
    double           m_delta;
    double           m_BsPrice;
    double           m_theta;
};

#endif // FUTUREOPTDATA_H
