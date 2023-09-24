#include "policy.h"
#include "job.h"
#include "limitedSet.h"
#include <set>

std::vector<Job*> IndexPolicy::choose(std::unordered_set<Job, JobHash> jobs, unsigned int k) {
    if (k >= jobs.size()) {
        return std::vector<Job*>();
    }

    LimitedSet chosen(k);
    for (const Job& j : jobs) {
        IndexedJob ijob(getIndex(j), &const_cast<Job&>(j));
        chosen.insert(ijob);
    }

    debug_print("chosen size is %ld, jobs size is %ld, k is %d\n", chosen.toVector().size(), jobs.size(), k);
    return chosen.toVector();
}

real FCFSPolicy::getIndex(Job job) {
    return job.arrivalTime;
}
