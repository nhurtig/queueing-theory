#ifndef JOB_H
#define JOB_H
#include "queue_standard.h"
#include "distribution.h"

class Job {
public:
    Job(Distribution*, real, jclass);
    Distribution *dist;
    real age;
    real arrivalTime;
    jclass job_class;
    real required;
    real nextInterrupt(real time);
};

class DeadJob {
public:
    DeadJob(Job job, real finishTime);
    Distribution *dist;
    real required;
    real arrivalTime;
    real finishTime;
    jclass job_class;
};

#endif
