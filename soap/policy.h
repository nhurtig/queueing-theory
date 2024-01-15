#ifndef POLICY_H
#define POLICY_H
#include "queue_standard.h"
#include "job.h"

class Policy {
public:
    virtual real getIndex(Job *job) = 0;
};

class FCFSPolicy: public Policy {
private:
    real getIndex(Job *job);
};

#endif
