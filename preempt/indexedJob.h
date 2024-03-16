#ifndef INDEXED_JOB_H
#define INDEXED_JOB_H
#include "queue_standard.h"
#include "policy.h"
#include "job.h"

class IndexedJob : public JobInterface {
public:
    IndexedJob(Policy *p, Job job);
    real index;
    Job job;

    real nextInterrupt() const;
    real getRequired() const;
    void serve(real time);
    void show() const;
    real getArrival() const;

    void addToService();
    void removeFromService();

    bool operator<(const IndexedJob& other) const;
    struct ReverseComparator {
        bool operator()(const IndexedJob& lhs, const IndexedJob &rhs) const;
    };
    unsigned int getID() const;
private:
    Policy *p;
};

#endif
