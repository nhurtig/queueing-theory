#include "policy.h"
#include "job.h"
#include <set>

// TODO: make this better. Use limited-size queue.
std::unordered_set<Job, JobHash> IndexPolicy::choose(std::unordered_set<Job, JobHash> jobs, unsigned int k) {
    if (k >= jobs.size()) {
        return jobs;
    }
    std::set<IndexedJob, decltype(compareJobs)*> orderedJobs;
    for (const Job& j : jobs) {
        orderedJobs.insert(IndexedJob(getIndex(j), j));
    }
    std::unordered_set<Job, JobHash> chosen;
    for (const IndexedJob& j : orderedJobs) {
        if (k-- <= 0) {
            break;
        }
        chosen.insert(j.job);
    }
    return chosen;
}

IndexedJob::IndexedJob(real index, Job job) : index{index}, job{job} {}

bool compareJobs(IndexedJob a, IndexedJob b) {
    return a.index > b.index;
}
