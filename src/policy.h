#ifndef POLICY_H
#define POLICY_H
#include "queue_standard.h"
#include "job.h"

class Policy {
public:
    virtual real getIndex(JobInterface *job) = 0;
    virtual ~Policy() {};
};

class FCFSPolicy: public Policy {
private:
    real getIndex(JobInterface *job);
};

class SRPTPreemptPolicy : public Policy {
public:
    SRPTPreemptPolicy(real alpha);
private:
    real alpha;
    real getIndex(JobInterface *job);
};

class SRPTPolicy : public SRPTPreemptPolicy {
public:
    SRPTPolicy();
};

#endif
