#include "limitedSet.h"

LimitedSet::LimitedSet(unsigned int k) {
    this->k = k;
    this->arr = (IndexedJob**) malloc(k*sizeof(IndexedJob*));
    this->size = 0;
}

void LimitedSet::insert(IndexedJob *job) {
    real index = job->index;
    if (this->size == this->k && this->arr[this->size-1]->index >= index) {
        // no need to insert
        return;
    }

    for (unsigned int i = 0; i < this->size; i++) {
        if (this->arr[i]->index < index) {
            this->insertInternal(job, i);
            return;
        }
    }

    if (this->size != this->k) {
        this->insertInternal(job, this->size);
    }
}

void LimitedSet::insertInternal(IndexedJob *job, unsigned int i) {
    if (i == this->size) {
        return;
    }

    IndexedJob *moved = this->arr[i];
    this->arr[i] = job;
    insertInternal(moved, i+1);
}

std::unordered_set<Job, JobHash> LimitedSet::toSet() {
    std::unordered_set<Job, JobHash> set;

    for (unsigned int i = 0; i < this->size; i++) {
        set.insert((*(this->arr[i])).job);
    }

    return set;
}
