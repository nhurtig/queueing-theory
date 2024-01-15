#include "policy.h"
#include "job.h"

real FCFSPolicy::getIndex(Job *job) {
    return job->arrivalTime;
}

real FCFSPolicy::timeTil(const Job *job, real bound) {
    return infinity;
}
