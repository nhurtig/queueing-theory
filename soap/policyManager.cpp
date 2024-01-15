#include "policyManager.h"
#include "policy.h"
#include "job.h"

PolicyManager::PolicyManager(unsigned int k, Policy *policy) : k(k), policy(policy) {
    this->hasChanged = false;
}

void PolicyManager::addJob(Job job) {
    this->hasChanged = true;
    this->queued.push(IndexedJob(this->policy, job));
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
    if (hasChanged) {
        recalculate();
    }
    // TODO
    return 1000;
}

void PolicyManager::serve(real time) {
    if (hasChanged) {
        recalculate();
    }

    hasChanged = true;

    // serve regular jobs
    serveEach(serving, time/k);

    // serve shared jobs
    int shareCount = sharedServing.size();
    if (shareCount != 0) { // shared jobs exist
        int serveCount = serving.size();
        real shareTime = (time/(k-serveCount))/shareCount;
        serveEach(sharedServing, shareTime);
    }

    return;
}

void PolicyManager::recalculate() {
    hasChanged = false;
    // put all jobs into a reverse-ordered priority queue
    std::priority_queue<IndexedJob, std::vector<IndexedJob>, IndexedJob::ReverseComparator> reverseQueue;
    // TODO
}

void PolicyManager::serveEach(std::vector<IndexedJob> toServe, real time) {
    auto it = toServe.begin();
    while (it != toServe.end()) {
        it->serve(time);
        if (it->done()) {
            completedJobs.push_back(it->job);
            it = toServe.erase(it);
        } else {
            it++;
        }
        it++;
    }

    return;
}
