#ifndef POLICY_MANAGER_H
#define POLICY_MANAGER_H
#include "queue_standard.h"
#include "job.h"
#include "policy.h"
#include <unordered_set>
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
    std::unordered_set<IndexedJob, IndexedJob::HashFunction> serving;
    std::unordered_set<IndexedJob, IndexedJob::HashFunction> sharedServing;
    std::priority_queue<IndexedJob> queued;
    std::unordered_set<Job, Job::HashFunction> completedJobs;
};

#endif
