#ifndef OTCOPTIONMODULE_H
#define OTCOPTIONMODULE_H

#include "./util/decodeinnermsg.h"
#include "./greek/futureoptpricing.h"

#include <string>
#include <vector>
#include <memory>
#include <set>
#include <unordered_map>
#include <QtCore>

class FutureOptData
{
public:
    FutureOptData();
    ~FutureOptData() = default;
    static void     addHoliday(QString str);
    void            setRiskVol(double vol);
    void            setRiskFreeRate(double rate);
    void            update(double price, otcOptPosition& opt);
    static double   calcCurElapseTime(QDate endDate);
    double          getRiskAmount();
    double          getOneGamma();
    double          getDelta();
    double          getBsPrice();
    double          getTheta();

private:
    double          calcAvoidRiskAmount(const otcOptPosition& opt) const;
    double          calcOneVolGamma(const otcOptPosition& opt) const;
    double          calcDelta(const otcOptPosition& opt) const;
    double          calcBsPrice(const otcOptPosition& opt) const;
    double          calcTheta(const otcOptPosition& opt) const;
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


class OtcOptionModule
{
public:
    OtcOptionModule();
    ~OtcOptionModule();

public:
    std::vector<otcOptPosition>     getOptPostionsByInst(std::string &inst);
    std::vector<otcOptPosition>     getOptPostionsByInstAndUser(std::string &inst, const std::string& user);
    void                            addOtcPosition(const otcOptPosition& pos);
    std::vector<otcOptPosition>     getAllOtcPositions();

private:
    std::unordered_map<std::string, otcOptPosition> m_otcPosInfos;

};

extern OtcOptionModule* GetOtcOptionModule();

#endif

