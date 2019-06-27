#ifndef FUTUREOPTPRICING_H
#define FUTUREOPTPRICING_H

namespace futureoptpricing
{
    double future_delta(char otcType, int direction, double S, double X, double T, double r, double v);
    double future_gamma(char otcType, int direction, double S, double X, double T, double r, double v);
    double future_otcprice(char otcType, int direction, double S, double X, double T, double r, double v);
    double future_theta(char otcType, int direction, double S, double X, double T, double r, double v);
    double future_vega(int direction, double S, double X, double T, double r, double v);
    double future_rho(char otcType, int direction, double S, double X, double T, double r, double v);
}

#endif // FUTUREOPTPRICING_H
