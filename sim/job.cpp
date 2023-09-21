#include "job.h"

Job::Job(Distribution *dist, real arrivalTime, jclass job_class) {
    this->dist = dist;
    this->arrivalTime = arrivalTime;
    this->required = dist->sample();
    this->age = 0;
    this->job_class = job_class;
}

real Job::nextInterrupt(real time) {
    return this->required - this->age;
}

DeadJob::DeadJob(Job job, real finishTime) {
    this->dist = job.dist;
    this->required = job.required;
    this->arrivalTime = job.arrivalTime;
    this->finishTime = finishTime;
    this->job_class = job.job_class;
}
