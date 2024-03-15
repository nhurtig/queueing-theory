#include "policy.h"
#include "job.h"

real FCFSPolicy::getIndex(IndexedJob *job) {
    return job->job->arrivalTime;
}

real SRPTPolicy::getIndex(IndexedJob *job) {
    // TODO
}

SRPTPreemptPolicy::SRPTPreemptPolicy(real alpha) {
    this->alpha = alpha;
}

