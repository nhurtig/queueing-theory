#include "job.h"

unsigned int Job::nextID = 0;

Job::Job(real arrivalTime, Distribution *dist) {
    this->arrivalTime = arrivalTime;
    this->required = dist->sample();
    this->age = 0;
    this->id = nextID++;
}

real Job::nextInterrupt() {
    return this->required - this->age;
}

void Job::serve(real time) {
    this->age += time;
}

real Job::getRequired() {
    return this->required;
}

std::size_t Job::HashFunction::operator()(const Job& job) const {
    return job.id;
}

bool Job::operator==(const Job& other) const {
    return this->id == other.id;
}

std::string DeadJob::sep = ",";
std::string DeadJob::lineSep = "\n";
std::string DeadJob::header = "FinishTime" + DeadJob::sep + "ArrivalTime" + DeadJob::sep + "ServiceTime" + DeadJob::sep + DeadJob::lineSep;

DeadJob::DeadJob(Job *job, real finishTime) {
    this->required = job->getRequired();
    this->arrivalTime = job->arrivalTime;
    this->finishTime = finishTime;
}

void DeadJob::toCSV(std::ofstream *stream) {
    *stream << this->finishTime << this->sep << this->arrivalTime << this->sep;
    *stream << this->required << this->lineSep;
}

IndexedJob::IndexedJob(real rank, Job job): rank{rank}, job{job} {}

std::size_t IndexedJob::HashFunction::operator()(const IndexedJob& ijob) const {
    return ijob.job.id;
}

bool IndexedJob::operator==(const IndexedJob& other) const {
    return this->job == other.job;
}

bool IndexedJob::operator<(const IndexedJob& other) const {
    if (this->rank < other.rank) {
        return true;
    } else if (this->rank > other.rank) {
        return false;
    } else {
        return this->job.arrivalTime < other.job.arrivalTime;
    }
}
