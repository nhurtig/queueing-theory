#include "policyManager.h"
#include "policy.h"
#include "job.h"
#include <iostream>

PolicyManager::PolicyManager(Policy *policy, real gamma) : policy(policy) {
    this->gamma = gamma;
    this->hasChanged = false;
}

void PolicyManager::addJob(Job job) {
    // debug_print("addJob start: %d\n", size());
    this->hasChanged = true;
    this->queued.push(IndexedJob(this->policy, job, gamma));
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

    if (serving != NULL) {
        return serving->nextInterrupt();
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

    if (serving != NULL) {
        serving->serve(time);
        if (serving->done()) {
            debug_print("job done! id %d\n", serving->getID());
            completedJobs.push_back(serving);
            serving = NULL;
        }
    }

    return;
}

void PolicyManager::recalculate() {
    // printf("recalc before: %d\n", size());
    show();
    hasChanged = false;
    if (this->isEmpty()) {
        return;
    }

    if (serving != NULL) {
        queued.push(serving);
    }
    serving = std::move(queued.top());
    queued.pop();

    return;
}

unsigned int PolicyManager::size() const {
    int servingSize = 0;
    if (serving != NULL) {
        servingSize++;
    }
    return servingSize + queued.size();
}

void PolicyManager::show() {
    return;
    std::cout << "serving: {";
    if (serving != NULL) {
        serving.show();
    }
    std::cout << "}\n";
    std::cout << "queued: {";
    for (const auto& job : queued) {
        job.show();
        std::cout << ", ";
    }
    std::cout << "}\n";
    std::cout << "completed: {";
    for (const auto& job : completedJobs) {
        job.show();
        std::cout << ", ";
    }
    std::cout << "}\n";
}
