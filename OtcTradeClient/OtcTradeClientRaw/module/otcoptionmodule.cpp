#include "otcoptionmodule.h"
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

OtcOptionModule::OtcOptionModule()
{
    m_impl = std::make_shared<OtcOptionModuleImpl>();
}

std::vector<otcOptPosition> OtcOptionModule::getOptPostionsByInst(std::string &inst)
{
    return m_impl->getOptPostionsByInst(inst);
}

std::vector<otcOptPosition> OtcOptionModule::getOptPostionsByInstAndUser(std::string &inst, const std::string &user)
{
    return m_impl->getOptPostionsByInstAndUser(inst, user);
}

void OtcOptionModule::addOtcPosition(const otcOptPosition &pos)
{
    m_impl->addOtcPosition(pos);
}

std::vector<otcOptPosition> OtcOptionModule::getAllOtcPositions()
{
    return m_impl->getAllOtcPositions();
}
