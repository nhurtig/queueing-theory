#include "policy.h"
#include "job.h"
#include "limitedSet.h"
#include <set>

std::vector<unsigned int> IndexPolicy::choose(std::map<unsigned int, Job> jobs, unsigned int k) {
    if (k >= jobs.size()) {
        return std::vector<unsigned int>();
    }

    LimitedSet chosen(k);
    for (const auto& pair : jobs) {
        IndexedJob ijob(getIndex(pair.second), pair.first);
        chosen.insert(ijob);
    }

    return chosen.toVector();
}

real FCFSPolicy::getIndex(Job job) {
    return -job.arrivalTime;
}
