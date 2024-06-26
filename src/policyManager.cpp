#include "policyManager.h"
#include "policy.h"
#include "job.h"
#include <iostream>

PolicyManager::PolicyManager(Policy *policy): hasChanged{false}, policy{policy} {}

// PolicyManagerConcrete::PolicyManagerConcrete(Policy *policy): PolicyManager(policy) {}

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

Job PolicyManager::getUnfinishedJob() {
    Job job = queued.top().job;
    queued.pop();
    return job;
}

real PolicyManager::nextInterrupt() {
    // debug_print("nI start\n");
    if (hasChanged) {
        recalculate();
    }
    if (serving) {
        return serving->nextInterrupt();
    }
    return infinity;
}

void PolicyManager::serve(real time) {
    // if (hasChanged) {
    //     printf("NO! BAD! how can the policyManager change without you asking me what my nextinterrupt is?\n");
    //     recalculate();
    // }

    hasChanged = true; // because the job might finish

    if (serving) {
        serving->serve(time);
        if (serving->done()) {
            debug_print("job done! id %d\n", serving->getID());
            completedJobs.push_back(serving->job);
            serving.reset();
        }
    }

    return;
}

// void PolicyManagerConcrete::recalculate() {
//     // printf("recalc before: %d\n", size());
//     hasChanged = false;

//     if (!serving) {
//         if (!queued.empty()) {
//             serving = std::make_unique<IndexedJob>(std::move(queued.top()));
//             queued.pop();
//             serving->addToService();
//         } // else everything is empty
//     } else {
//         if (!queued.empty()) {
//             if (*serving < queued.top()) { // is serving worse than what we could do?
//                 // printf("PREEMPT!\n");
//                 IndexedJob best = queued.top();
//                 queued.pop();
//                 serving->removeFromService();
//                 queued.push(*serving);
//                 serving = std::make_unique<IndexedJob>(std::move(best));
//                 serving->addToService();
//             } // else no swap is needed
//         } // else queued is empty, easy
//     }

//     return;
// }

unsigned int PolicyManager::size() const {
    return queued.size();
}

PolicyManagerPreempt::PolicyManagerPreempt(PreemptPolicy *policy): 
    PolicyManager(&this->srptPolicy),
    policy{policy},
    srptPolicy{SRPTPolicy()}
    {}

void PolicyManagerPreempt::recalculate() {
    hasChanged = false;

    if (!serving) {
        if (!queued.empty()) { // nobody is serving -- easy!
            serving = std::make_unique<IndexedJob>(std::move(queued.top()));
            queued.pop();
            serving->addToService();
        } // else everything is empty
    } else {
        if (!queued.empty()) {
            #ifdef PREEMPTION_UNINTERRUPTIBLE
            if (serving->getPreempt() > 0) {
                this->hasChanged = true;
            } else {
            #endif
                if (*serving < queued.top() && policy->preempt(*serving, queued)) {
                    // PREEMPT!
                    IndexedJob best = queued.top();
                    queued.pop();
                    serving->removeFromService();
                    queued.push(*serving);
                    serving = std::make_unique<IndexedJob>(std::move(best));
                    serving->addToService();
                }
            #ifdef PREEMPTION_UNINTERRUPTIBLE
            }
            #endif
        } // else no swap is needed
    } // else queued is empty, easy

    return;
}

// void PolicyManagerConcrete::show() {
//     return;
//     std::cout << "queued/serving: {";
//     // for (const auto& job : queued) { // TODO: figure out why this line is red
//         // job.show();
//         // std::cout << ", ";
//     // }
//     std::cout << "}\n";
//     std::cout << "completed: {";
//     for (const auto& job : completedJobs) {
//         job.show();
//         std::cout << ", ";
//     }
//     std::cout << "}\n";
// }
