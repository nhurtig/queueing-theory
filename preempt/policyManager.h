#ifndef POLICY_MANAGER_H
#define POLICY_MANAGER_H
#include "queue_standard.h"
#include "job.h"
#include "policy.h"
#include "indexedJob.h"
#include <queue>
#include <memory>

class PolicyManager {
public:
    PolicyManager(Policy *policy);
    void addJob(Job job);
    bool hasJob();
    Job getJob();
    void serve(real time);
    unsigned int size() const;
    real nextInterrupt();
private:
    bool hasChanged; // whether the served set needs recalculated
    Policy *policy;
    std::unique_ptr<IndexedJob> serving;
    std::priority_queue<IndexedJob> queued;
    std::vector<Job> completedJobs;
    void recalculate();
    void show();
};

#endif
