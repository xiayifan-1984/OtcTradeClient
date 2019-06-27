#ifndef BINOMIALDEFINE_H
#define BINOMIALDEFINE_H

//单期二叉树期权定价：
extern double option_price_call_european_binomial_single_period(const double& S, // spot price
        const double& K, // exercise price
        const double& r, // interest rate(per period)
        const double& u, // up movement
        const double& d); // down movement


extern double option_price_call_europern_binomial_multi_period_given_ud(const double& S, // spot price
         const double& K, // exercise price
         const double& r, // interest rate(per period)
         const double& u, // up movement
         const double& d, // down movement
         const int& no_periods); // no steps in binomial tree


#endif
