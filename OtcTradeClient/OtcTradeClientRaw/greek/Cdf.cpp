
#include "Cdf.h"

#include <cmath>
using namespace std;


//累积概率函数（标准正态分布） CDF
double Cdf::N(const double& z)
{
    if (z >  6.0) { return 1.0; }; // this guards against overflow

    if (z < -6.0) { return 0.0; };

    double b1 =  0.31938153;
    double b2 = -0.356563782;
    double b3 =  1.781477937;
    double b4 = -1.821255978;
    double b5 =  1.330274429;
    double p  =  0.2316419;
    double c2 =  0.3989423;     //它就是 1.0 / sqrt(2.0 * M_PI);

    double a= fabs(z);
    double t = 1.0/(1.0+a*p);
    double b = c2 * exp( -z * z / 2.0 );
    double n = ((((b5*t+b4)*t+b3)*t+b2)*t+b1)*t;

    n = 1.0-b*n;
    if ( z < 0.0 )
    {
        n = 1.0 - n;
    }

    return n;
};

double      Cdf::dN(const double& X)
{
    //double C = 1.0 / sqrt(2.0 * M_PI);
    double C = 0.3989423;

    if( X >100000000 || X < -100000000)
    {
        return  0.0;
    }

    return C * exp(-X * X / 2.0);
}

