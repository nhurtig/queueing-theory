#include "job.h"

Job::Job(Distribution *dist, real arrivalTime, jclass job_class) {
    this->dist = dist;
    this->arrivalTime = arrivalTime;
    this->required = dist->sample();
    this->age = 0;
    this->job_class = job_class;
}
