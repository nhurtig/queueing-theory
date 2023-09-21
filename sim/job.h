#ifndef JOB_H
#define JOB_H
#include "queue_standard.h"
#include "distribution.h"

class Job {
public:
    Job(Distribution*, real, jclass);
    Distribution *dist;
    real age;
    real required;
    real arrivalTime;
    jclass job_class;
};

#endif
