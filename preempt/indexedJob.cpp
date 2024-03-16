#include "indexedJob.h"
#include <iostream>

IndexedJob::IndexedJob(Policy *p, Job job): job{job}, p{p} {
    index = p->getIndex(this);
}

real IndexedJob::getRequired() const {
    return job.getRequired();
}

real IndexedJob::getArrival() const {
    return job.getArrival();
}

real IndexedJob::nextInterrupt() const {
    return job.nextInterrupt();
}

void IndexedJob::removeFromService() {
    this->job.removeFromService();
}

void IndexedJob::addToService() {
    this->job.addToService();
}

void IndexedJob::serve(real time) {
    this->job.serve(time);

    if (!job.done()) {
        index = p->getIndex(this);
        // printf("id %d: was age %Lf, rank %Lf, now age %Lf, rank %Lf\n", getID(), agebefore, rankbefore, job.age, rank);
    }
}

unsigned int IndexedJob::getID() const {
    return job.getID();
}

void IndexedJob::show() const {
    std::cout << "(";
    job.show();
    std::cout << ", " << index << ")";
}

bool IndexedJob::operator<(const IndexedJob& other) const {
    // returns TRUE if I'm less important (lower index) than other,
    // FALSE if other is more important
    if (this->index < other.index) {
        return true;
    } else if (this->index > other.index) {
        return false;
    } else {
        return this->job.arrivalTime > other.job.arrivalTime; // FCFS if all same
    }
}

bool IndexedJob::ReverseComparator::operator()(const IndexedJob& lhs, const IndexedJob& rhs) const {
    return lhs < rhs;
}
