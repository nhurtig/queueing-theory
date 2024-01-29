#ifndef POLICY_H
#define POLICY_H
#include "queue_standard.h"
#include "job.h"
#include <vector>

class Policy {
public:
    virtual real getIndex(Job *job) = 0;
    virtual real timeTil(const Job *job, real bound) = 0;
    virtual real timeTilIncrease(const Job *job) = 0;
};

class FCFSPolicy: public Policy {
private:
    real getIndex(Job *job);
    real timeTil(const Job *job, real bound);
    real timeTilIncrease(const Job *job);
};

class DiscreteGittinsPolicy: public Policy {
public:
    DiscreteGittinsPolicy(std::vector<real> vals, std::vector<real> probs);
private:
    real getIndex(Job *job);
    real timeTil(const Job *job, real bound);
    real timeTilIncrease(const Job *job);
    std::vector<real> vals;
    std::vector<real> probs;
    unsigned int size;
};

class DiscreteGittinsSlowdownPolicy: public Policy {
public:
    DiscreteGittinsSlowdownPolicy(std::vector<real> vals, std::vector<real> probs);
private:
    real getIndex(Job *job);
    real timeTil(const Job *job, real bound);
    real timeTilIncrease(const Job *job);
    std::vector<real> vals;
    std::vector<real> probs;
    unsigned int size;
};

#endif
