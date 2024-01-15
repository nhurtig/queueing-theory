#ifndef POLICY_MANAGER_H
#define POLICY_MANAGER_H
#include "queue_standard.h"
#include "job.h"
#include "policy.h"
#include "indexedJob.h"
#include <queue>

class PolicyManager {
public:
    PolicyManager(unsigned int k, Policy *policy);
    void addJob(Job job);
    bool hasJob();
    Job getJob();
    void serve(real time);
    real nextInterrupt();
private:
    unsigned int k; // how many servers
    bool hasChanged; // whether the served set needs recalculated
    Policy *policy;
    std::vector<IndexedJob> serving;
    std::vector<IndexedJob> sharedServing;
    std::priority_queue<IndexedJob> queued;
    std::vector<Job> completedJobs;
    void recalculate();
    void serveEach(std::vector<IndexedJob> toServe, real time);
};

#endif
