#ifndef POLICY_MANAGER_H
#define POLICY_MANAGER_H
#include "queue_standard.h"
#include "job.h"
#include "policy.h"
#include "preemptPolicy.h"
#include "indexedJob.h"
#include <queue>
#include <memory>

class PolicyManager {
public:
    PolicyManager(Policy *policy);
    unsigned int size() const;
    void addJob(Job job);
    virtual bool hasJob();
    virtual Job getJob();
    virtual Job getUnfinishedJob();
    void serve(real time);
    // virtual unsigned int size() const = 0;
    real nextInterrupt();
protected:
    virtual void recalculate() = 0;
    bool hasChanged;
    std::unique_ptr<IndexedJob> serving;
    std::priority_queue<IndexedJob> queued;
private:
    std::vector<Job> completedJobs;
    Policy *policy;
};

class PolicyManagerConcrete : public PolicyManager {
public:
    PolicyManagerConcrete(Policy *policy);
private:
    void recalculate();
};

class PolicyManagerPreempt : public PolicyManager {
public:
    PolicyManagerPreempt(PreemptPolicy *policy);
private:
    void recalculate();
    const PreemptPolicy *policy;
    SRPTPolicy srptPolicy;
};

#endif
