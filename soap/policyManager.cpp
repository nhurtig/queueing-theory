#include "policyManager.h"
#include "policy.h"
#include "job.h"

PolicyManager::PolicyManager(unsigned int k, Policy *policy) : k(k), policy(policy) {
    this->hasChanged = false;
}

void PolicyManager::addJob(Job job) {
    this->hasChanged = true;
    this->queued.push(IndexedJob(this->policy->getIndex(&job), job));
}

bool PolicyManager::hasJob() {
    return !completedJobs.empty();
}

Job PolicyManager::getJob() {
    Job job = *completedJobs.begin();
    completedJobs.erase(completedJobs.begin());
    return job;
}

real PolicyManager::nextInterrupt() {
    // TODO
    return 1000;
}

void PolicyManager::serve(real time) {
    // TODO
    return;
}
