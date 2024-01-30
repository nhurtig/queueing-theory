#include "policyManager.h"
#include "policy.h"
#include "job.h"
#include <iostream>

PolicyManager::PolicyManager(unsigned int k, Policy *policy) : k(k), policy(policy) {
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

    // how long until someone on the bench becomes good enough?
    // not using because we assume almost always weakly decreasing policy
    /*
    real bound;
    if (queued.empty()) {
        bound = infinity;
    } else {
        bound = queued.top().rank;
    }

    real interrupt = infinity;
    for (const auto& ijob : serving) {
        interrupt = std::min(interrupt, k*std::min(policy->timeTil(&(ijob.job), bound), ijob.getRequired()));
    }

    unsigned int sharedServers = k - serving.size();
    real multipilier = k * (sharedServing.size()/sharedServers);
    for (const auto& ijob : sharedServing) {
        interrupt = std::min(interrupt, multipilier*std::min(policy->timeTil(&(ijob.job), bound), ijob.getRequired()));
    }
    */

    unsigned int sharedServers = k - serving.size();
    real multiplier = infinity;
    if (sharedServers != 0) {
        multiplier = k * (sharedServing.size()/sharedServers);
    } else {
        multiplier = infinity;
    }

    // how long until a served job hits an increase?
    real interrupt = infinity;
    for (const auto& ijob : serving) {
        interrupt = std::min(interrupt, k*policy->timeTilIncrease(&(ijob.job)));
    }

    for (const auto& ijob : sharedServing) {
        interrupt = std::min(interrupt, multiplier*policy->timeTilIncrease(&(ijob.job)));
    }

    // how long until a served job finishes?
    for (const auto& ijob : serving) {
        interrupt = std::min(interrupt, k*ijob.getRequired());
    }

    for (const auto& ijob : sharedServing) {
        interrupt = std::min(interrupt, multiplier*ijob.getRequired());
    }

    // debug_print("nI end\n");
    // printf("interrupt=%Lf\n", interrupt);
    return interrupt;
}

void PolicyManager::serve(real time) {
    // debug_print("serve before: %d\n", size());
    if (hasChanged) {
        recalculate();
    }

    hasChanged = true;

    // serve regular jobs
    int serveCount = serving.size();
    serveEach(serving, time/k);

    // serve shared jobs
    int shareCount = sharedServing.size();
    if (shareCount != 0) { // shared jobs exist
        real shareTime = time*(k-serveCount)/(shareCount*k);
        serveEach(sharedServing, shareTime);
    }

    // debug_print("serve after: %d\n", size());
    return;
}

void PolicyManager::recalculate() {
    // printf("recalc before: %d\n", size());
    show();
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
        debug_print("recalc after: %d\n", size());
        show();
        return;
    }

    // put all served jobs into a priority queue
    std::priority_queue<IndexedJob, std::vector<IndexedJob>, IndexedJob::ReverseComparator> prevServed;
    while(!serving.empty()) {
        prevServed.push(std::move(serving.back()));
        serving.pop_back();
    }
    while(!sharedServing.empty()) {
        prevServed.push(std::move(sharedServing.back()));
        sharedServing.pop_back();
    }

    // move the things that got worse while serving
    // back into the queue (ensure prevServed >= queue)
    // printf("recalc canary: %d\n", size());
    while (!prevServed.empty() && prevServed.top() < queued.top()) {
        queued.push(std::move(prevServed.top()));
        prevServed.pop();
    }

    // now move best jobs into serving. Serving's back
    // is now the worst of the best (possibly more or less
    // than k things)
    while(!prevServed.empty()) {
        serving.push_back(std::move(prevServed.top()));
        prevServed.pop();
    }

    // bad and stupid step:
    // // pop off the least attractive served jobs until
    // // all jobs in serving are strictly better
    // // than all jobs in queued
    // while(!serving.empty() && serving.back() < queued.top()) {
    //     queued.push(std::move(serving.back()));
    //     serving.pop_back();
    // }

    // now grow/shrink best to the best k things, no
    // ties considered (yet)
    while (serving.size() < k) {
        serving.push_back(queued.top());
        queued.pop();
    }

    while(serving.size() > k) {
        queued.push(std::move(serving.back()));
        serving.pop_back();
    }

    // now guaranteed that serving has exactly k things.
    // serving has the best stuff (with the back being
    // the worst of the best) and queued
    // has the worst stuff (with the top being the best
    // of the worst).

    // NO SHARING CUZ DECREASING
    /*
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
    */

    // printf("recalc after: %d\n", size());
    show();
}

unsigned int PolicyManager::size() const {
    return serving.size() + sharedServing.size() + queued.size() + completedJobs.size();
}

void PolicyManager::serveEach(std::vector<IndexedJob>& toServe, real time) {
    // debug_print("serveEach start\n");
    show();
    auto it = toServe.begin();
    while (it != toServe.end()) {
        it->serve(time);
        if (it->done()) {
            debug_print("job done! id %d\n", it->job.getID());
            completedJobs.push_back(it->job);
            it = toServe.erase(it);
        } else {
            // debug_print("job not done!\n");
            it++;
        }
    }

    // debug_print("serveEach end\n");
    show();

    return;
}

void PolicyManager::show() {
    return;
    std::cout << "serving: {";
    for (const auto& job : serving) {
        job.show();
        std::cout << ", ";
    }
    std::cout << "}\n";
    std::cout << "shared: {";
    for (const auto& job : sharedServing) {
        job.show();
        std::cout << ", ";
    }
    std::cout << "}\n";
    // std::cout << "queued: {";
    // for (const auto& job : queued) {
    //     job.show();
    //     std::cout << ", ";
    // }
    // std::cout << "}\n";
    std::cout << "completed: {";
    for (const auto& job : completedJobs) {
        job.show();
        std::cout << ", ";
    }
    std::cout << "}\n";
}
