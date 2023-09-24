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

std::string DeadJob::sep = ",";
std::string DeadJob::lineSep = "\n";
std::string DeadJob::header = "FinishTime" + DeadJob::sep + "ArrivalTime" + DeadJob::sep + "ServiceTime" + DeadJob::sep + "JobClass" + DeadJob::lineSep;

DeadJob::DeadJob(Job job, real finishTime) {
    this->dist = job.dist;
    this->required = job.required;
    this->arrivalTime = job.arrivalTime;
    this->finishTime = finishTime;
    this->job_class = job.job_class;
}

void DeadJob::toCSV(std::ofstream *stream) {
    *stream << this->finishTime << this->sep << this->arrivalTime << this->sep;
    *stream << this->required << this->sep << this->job_class << this->lineSep;
}

bool operator==(const Job& lhs, const Job& rhs) {
    return lhs.id == rhs.id;
}

IndexedJob::IndexedJob(real index, Job job) : index{index}, job{job} {}
