#include "distribution.h"
#include <random>
#include <chrono>
#include <numeric>
#include <functional>
#include <stdexcept>

// from https://stackoverflow.com/questions/9878965/rand-between-0-and-1
uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
std::mt19937_64 generator = std::mt19937_64(ss);

std::uniform_real_distribution<long double> distribution(0.0, 1.0);

real rand_real() {
    return distribution(generator);
}

void seed_rand(int seed) {
    generator = std::mt19937_64(seed);
}

ExponentialDistribution::ExponentialDistribution(real lambda) {
    this->lambda = lambda;
}

real ExponentialDistribution::sample() {
    return -ln(-rand_real()+1)/this->lambda;
}

AdditiveExponentialDistribution::AdditiveExponentialDistribution(real lambda, real add): ExponentialDistribution(lambda), add{add} {}

real AdditiveExponentialDistribution::sample() {
    return this->add + ExponentialDistribution::sample();
}

// SuperDistribution::SuperDistribution(std::vector<std::unique_ptr<Distribution>> dists, std::vector<real> probs) {
//     real sum = std::accumulate(probs.begin(), probs.end(), 0.0);
//     std::for_each(probs.begin(), probs.end(), [sum](real x){return x/sum;});

//     this->size = dists.size();
//     if (this->size != probs.size()) {
//         throw std::invalid_argument("Distribution and probability vectors should be same length!");
//     }

//     this->dists = dists;
//     this->probs = probs;
// }

// real SuperDistribution::sample() {
//     real x = rand_real();
//     // debug_print("x=%Lf\n", x);
//     for (unsigned int i = 0; i < this->size; i++) {
//         if (x < probs[i]) {
//             return dists[i]->sample();
//         }
//         x -= probs[i];
//     }

//     // debug_print("after x=%Lf\n", x);
//     throw std::out_of_range("Did probs not add to 1?");
// }

std::unique_ptr<Distribution> makeExponentialDistribution(real lambda) {
    return std::make_unique<ExponentialDistribution>(lambda);
}

GoergHyperDistribution::GoergHyperDistribution(real w):
    d1{ExponentialDistribution(1+w)},
    d2{ExponentialDistribution(1-w)},
    p{(1+w)/2} {}

real GoergHyperDistribution::sample() {
    real x = rand_real();
    if (x < p) {
        return d1.sample();
    } else {
        return d2.sample();
    }
}

GoergHyperDistributionCB::GoergHyperDistributionCB(real cb): GoergHyperDistribution((cb*cb-1)/(cb*cb+1)) {}

DiscreteDistribution::DiscreteDistribution(std::vector<real> vals, std::vector<real> probs) {
    real sum = std::accumulate(probs.begin(), probs.end(), 0.0);
    std::for_each(probs.begin(), probs.end(), [sum](real x){return x/sum;});

    this->size = vals.size();
    if (this->size != probs.size()) {
        throw std::invalid_argument("Value and probability vectors should be same length!");
    }

    real lastx = 0;
    for (const auto& x : vals) {
        if (lastx >= x) {
            throw std::invalid_argument("Discrete values should be positive and increasing!");
        }
        lastx = x;
    }

    this->vals = vals;
    this->probs = probs;
}

real DiscreteDistribution::sample() {
    real x = rand_real();
    // debug_print("x=%Lf\n", x);
    for (unsigned int i = 0; i < this->size; i++) {
        if (x < probs[i]) {
            return vals[i];
        }
        x -= probs[i];
    }

    // debug_print("after x=%Lf\n", x);
    throw std::out_of_range("Did probs not add to 1?");
}

DegenerateDistribution::DegenerateDistribution(real x) {
    this->x = x;
}

real DegenerateDistribution::sample() {
    return x;
}

UniformDistribution::UniformDistribution(real min, real max) {
    this->min = min;
    this->max = max;
    if (min >= max) {
        throw std::out_of_range("Min should be less than max!");
    }
}

real UniformDistribution::cdf(real val) {
    if (val <= min) {
        return 0;
    } else if (val >= max) {
        return 1;
    } else {
        return (max-val)/(max-min);
    }
}

real UniformDistribution::sample() {
    real x = rand_real();
    return ((max-min)*x)+min;
}
