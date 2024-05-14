#include "policy.h"
#include "job.h"

real FCFSPolicy::getIndex(JobInterface *job) {
    return 1.0/job->getArrival();
}

SRPTPolicy::SRPTPolicy(): SRPTPreemptPolicy(0.0) {}

real SRPTPreemptPolicy::getIndex(JobInterface *job) {
    if (job->isInService()) {
        return 1.0/(job->getRequired() + job->getPreempt());
    } else {
        return 1.0/(job->getRequired() + job->getPreempt() + alpha);
    }
}

SRPTPreemptPolicy::SRPTPreemptPolicy(real alpha): alpha{alpha} {}
