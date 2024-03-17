#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H
#include "queue_standard.h"
#include <memory>
#include <vector>

real rand_real();

void seed_rand(int);

class Distribution {
public:
    // virtual real pdf(real) = 0;
    // virtual real cdf(real) = 0;
    virtual real sample() = 0;
};

class ExponentialDistribution: public Distribution {
    public:
        ExponentialDistribution(real);
        // real pdf(real);
        // real cdf(real);
        real sample();
    private:
        real lambda;
};

class AdditiveExponentialDistribution: public ExponentialDistribution {
    public:
        AdditiveExponentialDistribution(real lambda, real add);
        real sample();
    private:
        const real add;
};

// class SuperDistribution: public Distribution {
//     public:
//         SuperDistribution(std::vector<std::unique_ptr<Distribution>> dists, std::vector<real> probs);
//         real sample();
//     private:
//         unsigned int size;
//         std::vector<std::unique_ptr<Distribution>> dists;
//         std::vector<real> probs;
// };

class GoergHyperDistribution: public Distribution {
    public:
        GoergHyperDistribution(real w);
        real sample();
    private:
        ExponentialDistribution d1;
        ExponentialDistribution d2;
        const real p;
};

class GoergHyperDistributionCB: public GoergHyperDistribution {
    public:
        GoergHyperDistributionCB(real cb);
};

class DiscreteDistribution: public Distribution {
    public:
        DiscreteDistribution(std::vector<real> vals, std::vector<real> probs);
        // real cdf(real);
        real sample();
    private:
        unsigned int size;
        std::vector<real> vals;
        std::vector<real> probs;
};

class DegenerateDistribution: public Distribution {
    public:
        DegenerateDistribution(real x);
        // real cdf(real);
        real sample();
    private:
        real x;
};

class UniformDistribution: public Distribution {
    public:
        UniformDistribution(real min, real max);
        real cdf(real);
        real sample();
    private:
        real min;
        real max;
};

#endif
