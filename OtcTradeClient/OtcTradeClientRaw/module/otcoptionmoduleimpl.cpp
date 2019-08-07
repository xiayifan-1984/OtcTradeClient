#include "otcoptionmoduleimpl.h"
#include <string>
#include "stool.h"
#include "configmodule.h"
using namespace std;

OtcOptionModuleImpl::OtcOptionModuleImpl()
{

}

std::vector<otcOptPosition> OtcOptionModuleImpl::getOptPostionsByInst(string &inst)
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

std::vector<otcOptPosition> OtcOptionModuleImpl::getOptPostionsByInstAndUser(string &inst, const string &user)
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

void OtcOptionModuleImpl::addOtcPosition(const otcOptPosition &pos)
{
    auto loginName = stool::loginName();
    if(loginName != pos.trader) return;
    m_otcPosInfos.insert({pos.trade_no, pos});
}

std::vector<otcOptPosition> OtcOptionModuleImpl::getAllOtcPositions()
{
    vector<otcOptPosition> result;
    for(auto p = m_otcPosInfos.begin(); p!= m_otcPosInfos.end(); ++p)
    {
        result.push_back(p->second);
    }
    return result;
}

