#include "indexedJob.h"
#include <iostream>

IndexedJob::IndexedJob(Policy *p, Job job, real gamma): job{job}, p{p} {
    inService = false;
    gamma = gamma;
    preemptTime = gamma;
    rank = p->getIndex(this);
}

real IndexedJob::getRequired() const {
    return job.getRequired() + preemptTime;
}

void IndexedJob::serve(real time) {
    if (!inService) {
        printf("HEY! BAD! NO! don't serve jobs that aren't in service!\n");
    }
    // real agebefore = job.age;
    // real rankbefore = rank;
    if (this->preemptTime > 0) {
        time -= this->preemptTime;
        preemptTime = 0;
        if (time < 0) {
            preemptTime -= time;
        }
    }

    if (time > 0) {
        job.serve(time);
    }

    if (!job.done()) {
        rank = p->getIndex(this);
        // printf("id %d: was age %Lf, rank %Lf, now age %Lf, rank %Lf\n", getID(), agebefore, rankbefore, job.age, rank);
    }
    return;
}

unsigned int IndexedJob::getID() const {
    return job.getID();
}

void IndexedJob::show() const {
    std::cout << "(";
    job.show();
    std::cout << ", " << rank << ")";
}

void addToService() {
    this->inService = true;
}

void removeFromService() {
    this->inService = false;
    this->preemptTime = gamma;
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
