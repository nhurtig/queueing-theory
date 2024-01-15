#include "job.h"

bool JobInterface::done() const {
    return getRequired() <= 0;
}

std::size_t JobInterface::HashFunction::operator()(const JobInterface& job) const {
    return job.getID();
}

bool JobInterface::operator==(const JobInterface& other) const {
    return this->getID() == other.getID();
}

unsigned int Job::nextID = 0;

Job::Job(real arrivalTime, Distribution *dist) {
    this->arrivalTime = arrivalTime;
    this->required = dist->sample();
    this->age = 0;
    this->id = nextID++;
}

real Job::nextInterrupt() const {
    return this->required - this->age;
}

void Job::serve(real time) {
    this->age += time;
}

real Job::getRequired() const {
    return this->required;
}

unsigned int Job::getID() const {
    return id;
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

real IndexedJob::nextInterrupt() const {
    return job.nextInterrupt();
}

real IndexedJob::getRequired() const {
    return job.getRequired();
}

void IndexedJob::serve(real time) {
    job.serve(time);
    return;
}

unsigned int IndexedJob::getID() const {
    return job.getID();
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

bool IndexedJob::ReverseComparator::operator()(const IndexedJob& lhs, const IndexedJob& rhs) const {
    return lhs < rhs;
}
