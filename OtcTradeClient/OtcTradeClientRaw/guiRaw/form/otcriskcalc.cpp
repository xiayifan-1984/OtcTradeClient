#include "otcriskcalc.h"
#include "configmodule.h"

OtcRiskCalc::OtcRiskCalc(QWidget *parent)
{
    QConfigModule*  pConf = GetConfigModule();
    m_vol = pConf ? (pConf->g).vol : 0.2;
    m_otcOptData.setRiskVol(m_vol);
    m_otcOptData.setRiskFreeRate(pConf?(pConf->g).riskFreeRate:0.045);
    m_position = 0;
    memset(&m_curCode, 0, sizeof(tagXTInstrument));
}
