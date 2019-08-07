#ifndef DECODEINNERMSG_H
#define DECODEINNERMSG_H
#include <vector>
#include <QString>

class otcOptPosition
{
public:
    otcOptPosition();
    ~otcOptPosition() = default;
public:
    int                  status;
    std::string          trade_no;
    std::string         user_name;
    std::string           user_no;
    int                  direction;
    double                  volume;
    std::string        under_product;
    std::string        under_code;
    int                trade_date;
    int                 exec_type;
    int         option_struct;
    int         exec_date;
    double      exec_price;
    int         callput;
    int         enddate;
    double      price;
    double      under_price;
    double      multi;
    std::string        trader;
    double      under_sigma;
};

class otcOptPositionRsp
{
public:
    otcOptPositionRsp();
    ~otcOptPositionRsp() = default;
public:
    std::string resID;
    int  reqID;
    std::vector<otcOptPosition> positions;
};
namespace Decodeinnermsg
{
    bool decodeOtcOptPositionRsp(const char*, int len, otcOptPositionRsp&);
}

#endif // DECODEINNERMSG_H
