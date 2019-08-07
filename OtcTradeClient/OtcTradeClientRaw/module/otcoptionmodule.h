#ifndef OTCOPTIONMODULE_H
#define OTCOPTIONMODULE_H
#include "./util/decodeinnermsg.h"
#include "otcoptionmoduleimpl.h"
#include <string>
#include <vector>
#include <memory>

class OtcOptionModule
{
public:
    OtcOptionModule();
    ~OtcOptionModule();
    std::vector<otcOptPosition> getOptPostionsByInst(std::string &inst);
    std::vector<otcOptPosition> getOptPostionsByInstAndUser(std::string &inst, const std::string& user);
    void addOtcPosition(const otcOptPosition& pos);
    std::vector<otcOptPosition> getAllOtcPositions();
private:
    std::shared_ptr<OtcOptionModuleImpl> m_impl;
};

extern OtcOptionModule* GetOtcOptionModule();

#endif // OTCOPTIONMODULE_H
