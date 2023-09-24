#include "policy.h"
#include "job.h"
#include "limitedSet.h"
#include <set>

std::unordered_set<Job, JobHash> IndexPolicy::choose(std::unordered_set<Job, JobHash> jobs, unsigned int k) {
    if (k >= jobs.size()) {
        return jobs;
    }

    LimitedSet chosen(k);
    for (const Job& j : jobs) {
        IndexedJob ijob(getIndex(j), j);
        chosen.insert(&ijob);
    }

    return chosen.toSet();
}

real FCFSPolicy::getIndex(Job job) {
    return job.arrivalTime;
}
