
#include "blackscholes.h"
#include "Cdf.h"
#include <cmath>
using namespace std;


//欧式看涨期权：
double blackscholes::call_price(const double& S,
           const double& K,
           const double& r,
           const double& sigma,
           const double& time) // time to maturity
{
     double time_sqrt = sqrt(time);

     double d1 = (log(S/K)+r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;

     double d2 = d1 - sigma * time_sqrt;

     return S* Cdf::N(d1)-K*exp(-r*time)* Cdf::N(d2);
};

//欧式看跌期权：
double blackscholes::put_price(const double& S,
           const double& K,
           const double& r,
           const double& sigma,
           const double& time) // time to maturity
{
     double time_sqrt = sqrt(time);

     double d1 = (log(S/K) + r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;

     double d2 = d1 - sigma * time_sqrt;

     return K * exp(-r*time) * Cdf::N(-d2) - S * Cdf::N(-d1);
};


//二分法 通过期权价格计算 隐含波动率
double blackscholes::option_price_implied_volatility_call_bisection(const double& S,
            const double& K,
            const double& r,
            const double& time,
            const double& option_price)
{
     double sigma_low = 0.0001; // check for arbitrage violations
     double price = call_price(S,K,r,sigma_low,time);
     if( price > option_price)
     {
         return 0.0; // if price at almost zero volatility greater than price,return 0
     }

     // simple binomial search for the implied volatility
     // relies on the value of the option increasing in volatility
     const double ACCURACY = 1.0e-5; // make this smaller for higher accuracy
     const int MAX_ITERATIONS = 100;
     const double HIGH_VALUE = 1.0e10;
     const double ERROR = -1e40;

     // want to bracket sigma. first find a maximun sigma by finding a sigma
     // with a estimated price higher than the actual price.
     double sigma_high = 0.3;
     price = call_price(S,K,r,sigma_high,time);
     while(price < option_price)
     {
          sigma_high *=2.0; // keep doubling
          price = call_price(S,K,r,sigma_high,time);

          if(sigma_high > HIGH_VALUE)
          {
              return ERROR; // panic,something wrong.
          }
     }

     for(int i=0;i<MAX_ITERATIONS;i++)
     {
          double sigma = (sigma_low+sigma_high)*0.5;
          price = call_price(S,K,r,sigma,time);

          double test = (price-option_price);
          if(fabs(test)<ACCURACY)
          {
              return sigma;
          }

          if(test<0)
          {
              sigma_low = sigma;
          }
          else
          {
              sigma_high = sigma;
          }
     }

     return ERROR;
}

double blackscholes::call_delta(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& time)             //T =(t-t0)/365   t代表到期日  t0代表今天
{
    //N(d1)
    double time_sqrt = sqrt(time);

    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;

    double delta = Cdf::N(d1);
    return delta;
}

double blackscholes::call_theta(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& time)             //T =(t-t0)/365   t代表到期日  t0代表今天
{
    // (- S* dN(d1) * sigma)/(2*sqrt(T))  - r * K * exp(-r*T) * N(d2)

    double time_sqrt = sqrt(time);

    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;
    double d2 = d1 - sigma * time_sqrt;

    double p1 = (- S* Cdf::dN(d1) * sigma )/(2 * time_sqrt);
    double p2 = r * K * exp( -r * time) * Cdf::N( d2 );

    double theta = p1 - p2;
    return  theta;
}

double blackscholes::call_rho(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& time)             //T =(t-t0)/365   t代表到期日  t0代表今天
{
    // K * T * exp(-r*T) * N(d2)
    double time_sqrt = sqrt(time);

    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;
    double d2 = d1 - sigma * time_sqrt;

    double rho = K * time * exp( - r * time) * Cdf::N(d2);
    return rho;
}

double blackscholes::vega(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& time)             //T =(t-t0)/365   t代表到期日  t0代表今天
{
    //S* dN(d1) * sqrt(T)
    double time_sqrt = sqrt(time);

    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;

    double vega = S * Cdf::dN(d1) * time_sqrt;
    return vega;
}

double blackscholes::gamma(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& time)             //T =(t-t0)/365   t代表到期日  t0代表今天
{
    //dN(d1) / (S* sigma * sqrt(T) )
    double time_sqrt = sqrt(time);

    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;

    double gamma = Cdf::N(d1) / ( S * sigma * time_sqrt);
    return gamma;
}

double blackscholes::put_delta(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& time)             //T =(t-t0)/365   t代表到期日  t0代表今天
{
    //-N(-d1) = N(d1) -1
    double time_sqrt = sqrt(time);

    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;

    double delta = Cdf::N(d1) -1;
    return delta;
}

double blackscholes::put_theta(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& time)             //T =(t-t0)/365   t代表到期日  t0代表今天
{
    // - (S * dN(d1) * sigma)/2*sqrt(T) + r* K * exp(-r*T) * N(-d2)
    double time_sqrt = sqrt(time);

    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;
    double d2 = d1 - sigma * time_sqrt;

    double p1 = - (S * Cdf::dN(d1) * sigma) / (2* time_sqrt);
    double p2 = r * K * exp( - r* time) * Cdf::N( -d2);

    return p1 + p2;
}

double blackscholes::put_rho(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& time)             //T =(t-t0)/365   t代表到期日  t0代表今天
{
    // -K * T * exp(-r *T) * N( -d2)
    double time_sqrt = sqrt(time);

    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;
    double d2 = d1 - sigma * time_sqrt;
    double p2 = time * K * exp( - r* time) * Cdf::N( -d2);

    return -p2;
}
