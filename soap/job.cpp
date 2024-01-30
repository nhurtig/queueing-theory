#include "job.h"
#include <iostream>

bool JobInterface::done() const {
    return getRequired() <= DONE_TOLERANCE;
}

std::size_t JobInterface::HashFunction::operator()(const JobInterface& job) const {
    return job.getID();
}

bool JobInterface::operator==(const JobInterface& other) const {
    return this->getID() == other.getID();
}

unsigned int Job::nextID = 0;

Job::Job(real arrivalTime, real required) {
    this->arrivalTime = arrivalTime;
    this->required = required;
    this->age = 0;
    this->id = nextID++;
    debug_print("job with id %d created\n", id);
}

real Job::nextInterrupt() const {
    return this->required - this->age;
}

void Job::serve(real time) {
    this->age += time;
    this->required -= time;
    debug_print("id %d: %Lf remaining, done=%d\n", id, required, this->done());
}

real Job::getRequired() const {
    return this->required;
}

unsigned int Job::getID() const {
    return id;
}

void Job::show() const {
    std::cout << "(" << id << ", " << required << ")";
}

std::string DeadJob::sep = ",";
std::string DeadJob::lineSep = "\n";
std::string DeadJob::header = "FinishTime" + DeadJob::sep + "ArrivalTime" + DeadJob::sep + "ServiceTime" + DeadJob::sep + "ID" + DeadJob::lineSep;

DeadJob::DeadJob(Job *job, real finishTime) {
    this->serviceReq = job->age;
    this->arrivalTime = job->arrivalTime;
    this->finishTime = finishTime;
    this->id = job->id;
}

void DeadJob::toCSV(std::ofstream *stream) {
    *stream << this->finishTime << this->sep << this->arrivalTime << this->sep;
    *stream << this->serviceReq << this->sep << this->id << this->lineSep;
}
