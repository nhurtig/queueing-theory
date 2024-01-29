#include "policy.h"
#include "job.h"
#include <vector>
#include <numeric>
#include <functional>
#include <stdexcept>

real FCFSPolicy::getIndex(Job *job) {
    return job->arrivalTime;
}

real FCFSPolicy::timeTil(const Job *job, real bound) {
    return infinity;
}

real FCFSPolicy::timeTilIncrease(const Job *job) {
    return infinity;
}

DiscreteGittinsPolicy::DiscreteGittinsPolicy(std::vector<real> vals, std::vector<real> probs) {
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

real DiscreteGittinsPolicy::getIndex(Job *job) {
    // debug_print("DGP gI start\n");
    real a = job->age;
    unsigned int istar = 0;
    for (; istar < this->size; istar++) {
        if (this->vals[istar] > a) {
            break;
        }
    }
    if (istar >= this->size) {
        debug_print("a=%Lf\n", a);
        debug_print("done=%d\n", job->done());
        throw std::invalid_argument("Job is impossibly old!");
    }
    
    real Pprime = 0;
    for (unsigned int i = istar; i < this->size; i++) {
        Pprime += this->probs[i];
    }

    real Ga = infinity;
    for (unsigned int j = 0; j < this->size - istar; j++) {
        real num = 0;
        // debug_print("DGP gI canary j=%d\n", j);
        for (unsigned int i = istar; i <= istar + j; i++) {
            num += this->probs[i];
        }
        num /= Pprime;

        real denom = 0;
        for (unsigned int i = istar + j; i < this->size; i++) {
            denom += this->probs[i];
        }
        denom *= this->vals[istar + j];
        // debug_print("DGP gI canary istar+j-1=%d\n", istar+j-1);
        // if (istar + j > 0) {
        for (unsigned int i = istar; i < istar + j; i++) {
            denom += this->probs[i]*this->vals[i];
        }
        // }
        denom /= Pprime;
        denom -= a;

        real frac = denom/num;
        // debug_print("j=%d: frac=%Lf, num=%Lf, denom=%Lf\n", j, frac, num, denom);
        if (frac < Ga) {
            Ga = frac;
        }
    }
    
    // debug_print("DGP gI end\n");
    // debug_print("a=%Lf->%Lf\n", a, Ga);
    return Ga;
}

real DiscreteGittinsPolicy::timeTil(const Job *job, real bound) {
    //TODO
    throw std::invalid_argument("TODO: Not yet implemented!");
}

real DiscreteGittinsPolicy::timeTilIncrease(const Job *job) {
    for (real val : this->vals) {
        if (job->age < val) {
            return val - job->age;
        }
    }
    throw std::invalid_argument("Job is impossibly old!");
}

DiscreteGittinsSlowdownPolicy::DiscreteGittinsSlowdownPolicy(std::vector<real> vals, std::vector<real> probs) {
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

real DiscreteGittinsSlowdownPolicy::getIndex(Job *job) {
    // debug_print("DGP gI start\n");
    real a = job->age;
    unsigned int istar = 0;
    for (; istar < this->size; istar++) {
        if (this->vals[istar] > a) {
            break;
        }
    }
    if (istar >= this->size) {
        debug_print("a=%Lf\n", a);
        debug_print("done=%d\n", job->done());
        throw std::invalid_argument("Job is impossibly old!");
    }
    
    real Pprime = 0;
    for (unsigned int i = istar; i < this->size; i++) {
        Pprime += this->probs[i];
    }

    real Ga = infinity;
    for (unsigned int j = 0; j < this->size - istar; j++) {
        real num = 0;
        // debug_print("DGP gI canary j=%d\n", j);
        for (unsigned int i = istar; i <= istar + j; i++) {
            num += this->probs[i]/this->vals[i];
        }
        num /= Pprime;

        real denom = 0;
        for (unsigned int i = istar + j; i < this->size; i++) {
            denom += this->probs[i];
        }
        denom *= this->vals[istar + j];
        // debug_print("DGP gI canary istar+j-1=%d\n", istar+j-1);
        // if (istar + j > 0) {
        for (unsigned int i = istar; i < istar + j; i++) {
            denom += this->probs[i]*this->vals[i];
        }
        // }
        denom /= Pprime;
        denom -= a;

        real frac = denom/num;
        // debug_print("j=%d: frac=%Lf, num=%Lf, denom=%Lf\n", j, frac, num, denom);
        if (frac < Ga) {
            Ga = frac;
        }
    }
    
    // debug_print("DGP gI end\n");
    // debug_print("a=%Lf->%Lf\n", a, Ga);
    return Ga;
}

real DiscreteGittinsSlowdownPolicy::timeTil(const Job *job, real bound) {
    //TODO
    throw std::invalid_argument("TODO: Not yet implemented!");
}

real DiscreteGittinsSlowdownPolicy::timeTilIncrease(const Job *job) {
    for (real val : this->vals) {
        if (job->age < val) {
            return val - job->age;
        }
    }
    throw std::invalid_argument("Job is impossibly old!");
}
