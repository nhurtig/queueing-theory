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

    // obvious case: < k jobs
    if (serving.size() + sharedServing.size() + queued.size() < k) {
        // move everything to serving
        while(!sharedServing.empty()) {
            serving.push_back(std::move(sharedServing.back()));
            sharedServing.pop_back();
        }
        while(!queued.empty()) {
            serving.push_back(std::move(queued.top()));
            queued.pop();
        }
        return;
    }

    // put all served jobs into a priority queue
    std::priority_queue<IndexedJob> best;
    while(!serving.empty()) {
        best.push(std::move(serving.back()));
        serving.pop_back();
    }
    while(!sharedServing.empty()) {
        best.push(std::move(sharedServing.back()));
        sharedServing.pop_back();
    }

    // now move best jobs into serving. Serving's back
    // is now the worst of the best
    while(!best.empty()) {
        serving.push_back(std::move(best.top()));
        best.pop();
    }

    // pop off the least attractive served jobs until
    // all jobs in serving are strictly better
    // than all jobs in queued
    while(!serving.empty() && serving.back() < queued.top()) {
        queued.push(std::move(serving.back()));
        serving.pop_back();
    }

    // guaranteed that all jobs in serving are best.
    // now grow/shrink best to the best k things, no
    // ties considered (yet)
    while (serving.size() < k) {
        serving.push_back(queued.top());
        queued.pop();
    }

    std::vector<IndexedJob> between;
    while(serving.size() > k) {
        between.push_back(std::move(serving.back()));
        serving.pop_back();
    }

    // now guaranteed that serving has k things.
    // serving has the best stuff (with the back being
    // the worst of the best), between has the next best
    // stuff (with the back being the best), and queued
    // has the worst stuff (with the top being the best
    // of the worst).

    // The worst thing in serving is in an equivalence class.
    // That class should go into sharedServing.
    sharedServing.push_back(std::move(serving.back()));
    serving.pop_back();

    // Move equal things from serving into sharedServing
    while(!serving.empty() && sharedServing.back().closeTo(serving.back())) {
        sharedServing.push_back(std::move(serving.back()));
        serving.pop_back();
    }

    // Now move equal things from queued into sharedServing
    while(!queued.empty() && sharedServing.back().closeTo(queued.top())) {
        sharedServing.push_back(std::move(queued.top()));
        queued.pop();
    }
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
    }

    return;
}
