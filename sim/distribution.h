#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H
#include "queue_standard.h"

real rand_real();

void seed_rand(int);

class Distribution {
public:
    virtual real pdf(real) = 0;
    virtual real cdf(real) = 0;
    virtual real sample() = 0;
};

class ExponentialDistribution: public Distribution {
    public:
        ExponentialDistribution(real);
        real pdf(real);
        real cdf(real);
        real sample();
    private:
        real lambda;
};

#endif
