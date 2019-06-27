#ifndef BLACKSCHOLES_DEFINE_H
#define BLACKSCHOLES_DEFINE_H

namespace blackscholes
{

//欧式看涨期权：
double call_price(const double& S,      //标的 现价
           const double& K,             //执行价
           const double& r,             //r =ln(1+r0)   r0代表不连续的年化利率
           const double& sigma,         //波动率
           const double& T);            //T =(t-t0)/365   t代表到期日  t0代表今天

double call_delta(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& T);             //T =(t-t0)/365   t代表到期日  t0代表今天

double call_theta(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& T);             //T =(t-t0)/365   t代表到期日  t0代表今天

double call_rho(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& T);             //T =(t-t0)/365   t代表到期日  t0代表今天

double vega(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& T);             //T =(t-t0)/365   t代表到期日  t0代表今天

double gamma(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& T);             //T =(t-t0)/365   t代表到期日  t0代表今天

//欧式看跌期权：
double put_price(const double& S,
           const double& K,
           const double& r,
           const double& sigma,
           const double& time);


double put_delta(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& T);             //T =(t-t0)/365   t代表到期日  t0代表今天

double put_theta(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& T);             //T =(t-t0)/365   t代表到期日  t0代表今天

double put_rho(const double& S,              //标的 现价
                  const double& K,              //执行价
                  const double& r,              //r =ln(1+r0)   r0代表不连续的年化利率
                  const double& sigma,          //波动率
                  const double& T);             //T =(t-t0)/365   t代表到期日  t0代表今天












//二分法 通过期权价格计算 隐含波动率
double option_price_implied_volatility_call_bisection(const double& S,
            const double& K,
            const double& r,
            const double& time,
            const double& option_price);

};

#endif
