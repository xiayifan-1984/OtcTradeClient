
#include "binomial.h"
#include <cmath>
#include <algorithm> // defining the max() operator
using namespace std;

//单期二叉树期权定价：
double option_price_call_european_binomial_single_period(const double& S, // spot price
        const double& K, // exercise price
        const double& r, // interest rate(per period)
        const double& u, // up movement
        const double& d) // down movement
{
     double p_up = ( exp(r) - d )/( u - d );
     double p_down = 1.0 - p_up;

     double c_u = max(0.0, u*S - K);
     double c_d = max(0.0, d*S - K);

     double call_price = exp(-r)*(p_up*c_u + p_down*c_d);
     return call_price;
};


//多期二叉树期权定价：
#include <vector> // STL vector templates

double option_price_call_europern_binomial_multi_period_given_ud(const double& S, // spot price
         const double& K, // exercise price
         const double& r, // interest rate(per period)
         const double& u, // up movement
         const double& d, // down movement
         const int& no_periods) // no steps in binomial tree
{
     double Rinv = exp(-r); // invers of interest rate
     double ud = u/d;
     double p_up = (exp(r)-d)/(u-d);
     double p_down = 1.0 - p_up;

     vector<double> prices(no_periods+1);  // price of underlying security
     prices[0] = S*pow(d,no_periods);

     for(int i=1;i<=no_periods;++i)
     {
         prices[i] = ud * prices[i-1];
     }

     vector<double>call_values(no_periods+1); // value of corressponding call

     for(int i=0;i<=no_periods;++i)
     {
         call_values[i] = max(0.0, prices[i] - K);
     }

     for(int step=no_periods-1;step>=0;--step)
     {
         for(int i=0;i<=step;++i)
         {
             call_values[i] = (p_up*call_values[i+1] + p_down*call_values[i] )*Rinv;
         }
     }

     return call_values[0];
};


