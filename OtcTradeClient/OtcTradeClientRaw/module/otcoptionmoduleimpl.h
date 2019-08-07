#ifndef OTCOPTIONMODULEIMPL_H
#define OTCOPTIONMODULEIMPL_H
#include "./util/decodeinnermsg.h"
#include <unordered_map>
#include <string>

class OtcOptionModuleImpl
{
public:
    OtcOptionModuleImpl();
    ~OtcOptionModuleImpl() = default;
    std::vector<otcOptPosition> getOptPostionsByInst(std::string& inst);
    std::vector<otcOptPosition> getOptPostionsByInstAndUser(std::string &inst, const std::string& user);
    void addOtcPosition(const otcOptPosition& pos);
    std::vector<otcOptPosition> getAllOtcPositions();
private:
    std::unordered_map<std::string, otcOptPosition> m_otcPosInfos;
};

#endif // OTCOPTIONMODULEIMPL_H
