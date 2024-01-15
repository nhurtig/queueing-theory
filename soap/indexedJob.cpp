#include "indexedJob.h"

IndexedJob::IndexedJob(Policy *p, Job job): job{job}, p{p} {
    rank = p->getIndex(&job);
}

real IndexedJob::nextInterrupt() const {
    return job.nextInterrupt();
}

real IndexedJob::getRequired() const {
    return job.getRequired();
}

void IndexedJob::serve(real time) {
    job.serve(time);
    rank = p->getIndex(&job);
    return;
}

unsigned int IndexedJob::getID() const {
    return job.getID();
}

bool IndexedJob::operator<(const IndexedJob& other) const {
    if (this->rank < other.rank) {
        return true;
    } else if (this->rank > other.rank) {
        return false;
    } else {
        return this->job.arrivalTime < other.job.arrivalTime;
    }
}

bool IndexedJob::ReverseComparator::operator()(const IndexedJob& lhs, const IndexedJob& rhs) const {
    return lhs < rhs;
}
