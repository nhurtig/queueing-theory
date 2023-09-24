#ifndef POLICY_H
#define POLICY_H
#include "queue_standard.h"
#include "job.h"
#include <vector>
#include <map>

class Policy {
public:
    virtual std::vector<unsigned int> choose(std::map<unsigned int, Job> jobs, unsigned int k) = 0;
};

class IndexPolicy: public Policy {
public:
    std::vector<unsigned int> choose(std::map<unsigned int, Job> jobs, unsigned int k);
private:
    virtual real getIndex(Job job) = 0;
};

class FCFSPolicy: public IndexPolicy {
private:
    real getIndex(Job job);
};

bool compareJobs(IndexedJob a, IndexedJob b);

#endif
