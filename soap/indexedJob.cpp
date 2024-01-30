#include "indexedJob.h"
#include <iostream>

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
    // real agebefore = job.age;
    // real rankbefore = rank;
    job.serve(time);
    if (!job.done()) {
        rank = p->getIndex(&job);
        // printf("id %d: was age %Lf, rank %Lf, now age %Lf, rank %Lf\n", getID(), agebefore, rankbefore, job.age, rank);
    }
    return;
}

bool IndexedJob::closeTo(const IndexedJob other) const {
    return rank == other.rank && job.age == other.job.age;
}

unsigned int IndexedJob::getID() const {
    return job.getID();
}

void IndexedJob::show() const {
    std::cout << "(";
    job.show();
    std::cout << ", " << rank << ")";
}

bool IndexedJob::operator<(const IndexedJob& other) const {
    // returns TRUE if I'm less important (higher rank) than other,
    // FALSE if other is more important
    if (this->rank < other.rank) {
        return false;
    } else if (this->rank > other.rank) {
        return true;
    } else {
        return this->job.arrivalTime > other.job.arrivalTime;
    }
}

bool IndexedJob::ReverseComparator::operator()(const IndexedJob& lhs, const IndexedJob& rhs) const {
    return lhs < rhs;
}
