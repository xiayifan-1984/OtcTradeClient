#include "otcoptionmoduleimpl.h"
#include <string>
using namespace std;

OtcOptionModuleImpl::OtcOptionModuleImpl()
{

}

std::vector<otcOptPosition> OtcOptionModuleImpl::getOptPostionsByInst(string &inst)
{
    vector<otcOptPosition> result;
    for(auto p = m_otcPosInfos.begin(); p!= m_otcPosInfos.end(); ++p)
    {
        if(inst == p->second.under_code)
        {
            result.push_back(p->second);
        }
    }
    return result;
}

void OtcOptionModuleImpl::addOtcPosition(const otcOptPosition &pos)
{
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

