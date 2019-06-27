#include "futureoptpricing.h"
#include <cmath>
#include "Cdf.h"

double futureoptpricing::future_delta(char otcType, int direction, double S, double X, double T, double r, double v)
{
    if(T == 0.0 || v == 0.0) return 0;
    double d1 = (log(S/X)+(v*v/2.0)*T)/(v*sqrt(T));
    if(otcType == 'c')
    {
        return exp(-1.0*r*T)*Cdf::N(d1)*direction;
    }
    else
    {
        return exp(-1*r*T)*(Cdf::N(d1)-1)*direction;
    }
}



double futureoptpricing::future_gamma(char otcType, int direction, double S, double X, double T, double r, double v)
{
    if(T == 0.0 || v == 0.0) return 0;
    double d1 = (log(S/X)+(v*v/2.0)*T)/(v*sqrt(T));
    return exp(-1.0*r*T)*(Cdf::dN(d1))/(S*v*sqrt(T))*direction;
}

double futureoptpricing::future_otcprice(char otcType, int direction, double S, double X, double T, double r, double v)
{
    if(T == 0.0 || v == 0.0) return 0;
    double d1 = (log(S/X)+(v*v/2)*T)/(v*sqrt(T));
    double d2 = d1 - v*sqrt(T);
    if(otcType == 'c')
    {
        return S*exp(-1*r*T)*Cdf::N(d1) - X*exp(-1*r*T)*Cdf::N(d2);
    }
    else
    {
       return X*exp(-1*r*T)*Cdf::N(-1*d2) - S* exp(-1*r*T)*Cdf::N(-d1);
    }
}



double futureoptpricing::future_theta(char otcType, int direction, double S, double X, double T, double r, double v)
{
    if(T == 0.0 || v == 0.0) return 0;
    double d1 = (log(S/X)+(v*v/2)*T)/(v*sqrt(T));
    double d2 = d1 - v*sqrt(T);

    if(otcType == 'c')
    {
        return (-1.0*S*exp(-1.0*r*T)*Cdf::dN(d1)*v/(2.0*sqrt(T)) - (-1.0*r)*S*exp(-1.0*r*T)*Cdf::N(d1)-r*X*exp(-1.0*r*T)*Cdf::N(d2))*direction/245;
    }
    else
    {
        return (-1.0*S*exp(-1.0*r*T)*Cdf::dN(d1)*v/(2.0*sqrt(T)) + (-1.0*r)*S*exp(-1.0*r*T)*Cdf::N(-1.0*d1) + r*X*exp(-1.0*r*T)*Cdf::N(-1.0*d2))*direction/245;
    }
}



double futureoptpricing::future_vega(int direction, double S, double X, double T, double r, double v)
{
    if(T == 0.0 || v == 0.0) return 0;
    double d1 = (log(S/X)+(v*v/2)*T)/(v*sqrt(T));
    return direction/100.0*(S*exp(-1.0*r*T)*Cdf::dN(d1)*sqrt(T));
}

double futureoptpricing::future_rho(char otcType, int direction, double S, double X, double T, double r, double v)
{
    if(T == 0.0 || v == 0.0) return 0;
    double d1 = (log(S/X)+(v*v/2)*T)/(v*sqrt(T));
    double d2 = d1 - v*sqrt(T);

    return -1.0*T*future_otcprice(otcType, direction, S, X, T, r, v)*direction/100.0;
}
