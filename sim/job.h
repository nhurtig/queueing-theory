#ifndef JOB_H
#define JOB_H
#include "queue_standard.h"
#include "distribution.h"

class Job {
public:
    static unsigned int nextID;
    Job(Distribution*, real, jclass);
    Distribution *dist;
    real age;
    real arrivalTime;
    jclass job_class;
    real required;
    unsigned int id;
    real nextInterrupt();
};

class DeadJob {
public:
    DeadJob(Job, real);
    Distribution *dist;
    real required;
    real arrivalTime;
    real finishTime;
    jclass job_class;
};

struct JobHash {
    size_t operator() (const Job& j) const {
        return j.id;
    }
};

class IndexedJob {
public:
    IndexedJob(real, Job);
    real index;
    Job job;
};

bool operator==(const Job& lhs, const Job& rhs);

#endif
