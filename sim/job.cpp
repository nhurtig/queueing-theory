#include "job.h"

unsigned int Job::nextID = 0;

Job::Job(Distribution *dist, real arrivalTime, jclass job_class) {
    this->dist = dist;
    this->arrivalTime = arrivalTime;
    this->required = dist->sample();
    this->age = 0;
    this->job_class = job_class;
    this->id = nextID++;
}

real Job::nextInterrupt() {
    return this->required - this->age;
}

DeadJob::DeadJob(Job job, real finishTime) {
    this->dist = job.dist;
    this->required = job.required;
    this->arrivalTime = job.arrivalTime;
    this->finishTime = finishTime;
    this->job_class = job.job_class;
}

bool operator==(const Job& lhs, const Job& rhs) {
    return lhs.id == rhs.id;
}
