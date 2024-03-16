#include "policyManager.h"
#include "policy.h"
#include "job.h"
#include <iostream>

PolicyManager::PolicyManager(Policy *policy) : policy(policy) {
    this->hasChanged = false;
}

void PolicyManager::addJob(Job job) {
    // debug_print("addJob start: %d\n", size());
    this->hasChanged = true;
    this->queued.push(IndexedJob(this->policy, job));
    // debug_print("addJob end: %d\n", size());
}

bool PolicyManager::hasJob() {
    return !completedJobs.empty();
}

Job PolicyManager::getJob() {
    Job job = completedJobs.back();
    completedJobs.pop_back();
    return job;
}

real PolicyManager::nextInterrupt() {
    // debug_print("nI start\n");
    if (hasChanged) {
        recalculate();
    }
    if (!queued.empty()) {
        return queued.top().nextInterrupt();
    }
    return infinity;
}

void PolicyManager::serve(real time) {
    // debug_print("serve before: %d\n", size());
    if (hasChanged) {
        printf("NO! BAD! how can the policyManager change without you asking me what my nextinterrupt is?\n");
        recalculate();
    }

    hasChanged = true; // because the job might finish

    if (!queued.empty()) {
        // queued.top
        IndexedJob serving = queued.top();
        queued.pop();
        serving.serve(time);
        // queued.top().serve(time);
        if (serving.done()) {
            debug_print("job done! id %d\n", serving->getID());
            completedJobs.push_back(serving.job);
        } else {
            queued.push(serving);
        }
    }

    return;
}

void PolicyManager::recalculate() {
    // printf("recalc before: %d\n", size());
    show();
    hasChanged = false;
    // everything's in one sorted structure!
    return;
}

unsigned int PolicyManager::size() const {
    return queued.size();
}

void PolicyManager::show() {
    return;
    std::cout << "queued/serving: {";
    // for (const auto& job : queued) { // TODO: figure out why this line is red
    //     job.show();
    //     std::cout << ", ";
    // }
    std::cout << "}\n";
    std::cout << "completed: {";
    for (const auto& job : completedJobs) {
        job.show();
        std::cout << ", ";
    }
    std::cout << "}\n";
}
