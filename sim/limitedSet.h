#ifndef LIMITEDSET_H
#define LIMITEDSET_H
#include "queue_standard.h"
#include "job.h"
#include <unordered_set>

class LimitedSet {
public:
    LimitedSet(unsigned int k);
    void insert(IndexedJob *job);
    std::unordered_set<Job, JobHash> toSet();
private:
    IndexedJob** arr;
    unsigned int k;
    unsigned int size;
    void insertInternal(IndexedJob *job, unsigned int i);
};

#endif
