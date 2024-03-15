#ifndef POLICY_H
#define POLICY_H
#include "queue_standard.h"
#include "job.h"

class Policy {
public:
    virtual real getIndex(IndexedJob *job) = 0;
};

class FCFSPolicy: public Policy {
private:
    real getIndex(Job *job);
};

class SRPTPolicy : public Policy {
private:
    real getIndex(Job *job);
}

class SRPTPreemptPolicy : public Policy {
public:
    SRPTPreemptPolicy(real alpha);
private:
    real alpha;
    real getIndex(Job *job);
}

#endif
