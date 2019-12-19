
#include "otcoptionmodule.h"
#include "stool.h"
using namespace std;

//..................................................................................................................................................................................
//..................................................................................................................................................................................
//..................................................................................................................................................................................
//..................................................................................................................................................................................

OtcOptionModule* g_otcOpt = nullptr;
OtcOptionModule* GetOtcOptionModule()
{
    if(g_otcOpt)
    {
        return g_otcOpt;
    }
    g_otcOpt =  new OtcOptionModule();
    return g_otcOpt;
}

//..................................................................................................................................................................................
//..................................................................................................................................................................................
//..................................................................................................................................................................................
//..................................................................................................................................................................................

const double workdays_per_year = 245.0;
static inline int tdirection(int dir)
{
    return dir == 0 ? -1 : 1; // dir is customer direction and transformed as trader direction
}

static inline char callput(int type)
{
    return type == 0 ? 'c' : 'p';
}

FutureOptData::FutureOptData()
{
    m_riskVol = 0.2;
    m_riskFreeRate = 0.01;
    m_avoidRiskAmount = 0.0;
    m_gammaForOneVol = 0.0;
    m_elapseTime = 0.01;
    m_curPrice = 0.0;
    m_theta = 0.0;
}

std::set<QDate> FutureOptData::m_holidays;

void FutureOptData::addHoliday(QString str)
{
    m_holidays.insert(QDate::fromString(str, "yyyyMMdd"));
}

void FutureOptData::setRiskVol(double vol)
{
    m_riskVol = vol;
}

void FutureOptData::setRiskFreeRate(double rate)
{
    m_riskFreeRate = rate;
}

void FutureOptData::update(double price, otcOptPosition &opt)
{
    m_curPrice = price;
    m_avoidRiskAmount = calcAvoidRiskAmount(opt);
    m_gammaForOneVol = calcOneVolGamma(opt);
    m_delta = calcDelta(opt);
    m_BsPrice = calcBsPrice(opt);
    m_theta = calcTheta(opt);
}

double FutureOptData:: calcCurElapseTime(QDate endDate)
{
    QDate beginDate(QDate::currentDate());
    auto isWorkingDay = [&](QDate d)
    {
        auto search = m_holidays.find(d);
        return search == m_holidays.end() && d.dayOfWeek() != 6 && d.dayOfWeek() != 7;
    };
    if(!isWorkingDay(endDate) || beginDate.daysTo(endDate)<0)
    {
        return -1;
    }
    QDate bDate = beginDate;
    QDate eDate = endDate;

    while(!isWorkingDay(bDate))
    {
        bDate = bDate.addDays(1);
    }

    int nWorkingDay = 0;
    while(bDate != eDate)
    {
        if(isWorkingDay(bDate))
        {
            ++nWorkingDay;
        }
        bDate = bDate.addDays(1);
    }

    QTime curTime = QTime::currentTime();
    if(!isWorkingDay(beginDate))
    {
        curTime = QTime::fromString("00:00:00", "hh:mm:ss");
    }
    QTime closeTime = QTime::fromString("15:00:00", "hh:mm:ss");
    double timeToClose = curTime.secsTo(closeTime);
    return timeToClose/3600.0/24.0 + (double)(nWorkingDay);
}

double FutureOptData::getRiskAmount()
{
    return m_avoidRiskAmount;
}

double FutureOptData::getOneGamma()
{
    return  m_gammaForOneVol;
}

double FutureOptData::getDelta()
{
    return m_delta;
}

double FutureOptData::getBsPrice()
{
    return m_BsPrice;
}

double FutureOptData::getTheta()
{
    return m_theta;
}

double FutureOptData::calcAvoidRiskAmount(const otcOptPosition& opt) const
{
    double T = FutureOptData::calcCurElapseTime(QDate::fromString(QString::number(opt.enddate), "yyyyMMdd"))/workdays_per_year;
    double delta = futureoptpricing::future_delta(callput(opt.callput), tdirection(opt.direction), m_curPrice, opt.exec_price, T, m_riskFreeRate, m_riskVol);
    return delta*m_curPrice*opt.volume*opt.multi;
}

double FutureOptData::calcOneVolGamma(const otcOptPosition &opt) const
{
    double T = FutureOptData::calcCurElapseTime(QDate::fromString(QString::number(opt.enddate), "yyyyMMdd"))/workdays_per_year;
    double gamma = futureoptpricing::future_gamma('x', tdirection(opt.direction), m_curPrice, opt.exec_price, T, m_riskFreeRate, m_riskVol);
    double enterVol = opt.under_sigma;
    //double enterVol = 0.18;
    return m_curPrice*m_curPrice*gamma*opt.volume*opt.multi/100.0*enterVol/sqrt(workdays_per_year)*100.0;
}

double FutureOptData::calcDelta(const otcOptPosition &opt) const
{
    double T = FutureOptData::calcCurElapseTime(QDate::fromString(QString::number(opt.enddate), "yyyyMMdd"))/workdays_per_year;
    double delta = futureoptpricing::future_delta(callput(opt.callput), tdirection(opt.direction), m_curPrice, opt.exec_price, T, m_riskFreeRate, m_riskVol);
    return delta;
}

double FutureOptData::calcBsPrice(const otcOptPosition &opt) const
{
    double T = FutureOptData::calcCurElapseTime(QDate::fromString(QString::number(opt.enddate), "yyyyMMdd"))/workdays_per_year;
    double price = futureoptpricing::future_otcprice(callput(opt.callput), tdirection(opt.direction), m_curPrice, opt.exec_price, T, m_riskFreeRate, m_riskVol);
    return price;
}

double FutureOptData::calcTheta(const otcOptPosition &opt) const
{
    double T = FutureOptData::calcCurElapseTime(QDate::fromString(QString::number(opt.enddate), "yyyyMMdd"))/workdays_per_year;
    double theta = futureoptpricing::future_theta(callput(opt.callput), tdirection(opt.direction), m_curPrice, opt.exec_price, T, m_riskFreeRate, m_riskVol);
    return  theta;
}

//..................................................................................................................................................................................
//..................................................................................................................................................................................
//..................................................................................................................................................................................
//..................................................................................................................................................................................
OtcOptionModule::OtcOptionModule()
{

}

OtcOptionModule::~OtcOptionModule()
{

}

std::vector<otcOptPosition> OtcOptionModule::getOptPostionsByInst(string &inst)
{
    vector<otcOptPosition> result;
    auto lower = stool::strToLower(inst);
    for(auto p = m_otcPosInfos.begin(); p!= m_otcPosInfos.end(); ++p)
    {
        if(inst == p->second.under_code || lower == p->second.under_code)
        {
            result.push_back(p->second);
        }
    }
    return result;
}

std::vector<otcOptPosition> OtcOptionModule::getOptPostionsByInstAndUser(string &inst, const string &user)
{
    vector<otcOptPosition> result;
    for(auto p = m_otcPosInfos.begin(); p!=m_otcPosInfos.end(); ++p)
    {
        if(inst == p->second.under_code && user == p->second.trader)
        {
            result.push_back(p->second);
        }
    }
    return result;
}

void OtcOptionModule::addOtcPosition(const otcOptPosition &pos)
{
    auto loginName = stool::loginName();
    if(loginName != pos.trader) return;

    m_otcPosInfos.insert({pos.trade_no, pos});
}

std::vector<otcOptPosition> OtcOptionModule::getAllOtcPositions()
{
    vector<otcOptPosition> result;
    for(auto p = m_otcPosInfos.begin(); p!= m_otcPosInfos.end(); ++p)
    {
        result.push_back(p->second);
    }
    return result;
}


















