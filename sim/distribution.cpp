#include "distribution.h"
#include <random>
#include <chrono>

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

real ExponentialDistribution::pdf(real x) {
    return this->lambda*exp(-this->lambda*x);
}

real ExponentialDistribution::cdf(real x) {
    return 1-exp(-this->lambda*x);
}

real ExponentialDistribution::sample() {
    return -ln(-rand_real()+1)/this->lambda;
}
