#include "limitedSet.h"

LimitedSet::LimitedSet(unsigned int k) {
    this->k = k;
    this->arr = (IndexedJob*) malloc(k*sizeof(IndexedJob));
    this->size = 0;
}

LimitedSet::~LimitedSet() {
    free(this->arr);
}

void LimitedSet::insert(IndexedJob job) {
    real index = job.index;
    if (this->size == this->k && this->arr[this->size-1].index >= index) {
        // no need to insert
        return;
    }

    bool inserted = false;
    for (unsigned int i = 0; i < this->size; i++) {
        if (this->arr[i].index < index) {
            this->insertInternal(job, i);
            inserted = true;
            break;
        }
    }

    if (this->size != this->k) {
        if (!inserted) {
            this->insertInternal(job, this->size);
        }
        this->size++;
    }
}

void LimitedSet::insertInternal(IndexedJob job, unsigned int i) {
    if (i == k) { // bumped out
        return;
    }

    IndexedJob moved = this->arr[i];
    this->arr[i] = job;
    insertInternal(moved, i+1);
}

std::vector<unsigned int> LimitedSet::toVector() {
    std::vector<unsigned int> vector;

    for (unsigned int i = 0; i < this->size; i++) {
        vector.push_back((this->arr[i]).id);
    }

    return vector;
}
