#include "policy.h"
#include "job.h"

real FCFSPolicy::getIndex(JobInterface *job) {
    return 1.0/job->getArrival();
}

real SRPTPolicy::getIndex(JobInterface *job) {
    return 1.0/job->getRequired();
    // return 1.0;
}

real SRPTPreemptPolicy::getIndex(JobInterface *job) {
    return 1.0/(job->getRequired() + alpha);
}

SRPTPreemptPolicy::SRPTPreemptPolicy(real alpha) {
    this->alpha = alpha;
}

