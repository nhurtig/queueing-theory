#include "policy.h"
#include "job.h"

real FCFSPolicy::getIndex(Job *job) {
    return -job->arrivalTime;
}
