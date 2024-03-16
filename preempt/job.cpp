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

Job::Job(real arrivalTime, real required, real gamma): gamma{gamma} {
    this->arrivalTime = arrivalTime;
    this->required = required;
    this->age = 0;
    this->id = nextID++;
    this->preemptTime = gamma;
    this->inService = false;
    debug_print("job with id %d created\n", id);
}

real Job::nextInterrupt() const { // TODO: is this a bug?
    return this->required + this->preemptTime;
}

void Job::addToService() {
    this->inService = true;
}

void Job::removeFromService() {
    this->inService = false;
    this->preemptTime = gamma;
}

void Job::serve(real time) {
    if (!inService) {
        printf("HEY! BAD! NO! don't serve jobs that aren't in service!\n");
    }
    // real agebefore = job.age;
    // real rankbefore = rank;
    // debug_print("serving for %Lf. Preempt=%Lf, req=%Lf, age=%Lf, id=%d\n", time, preemptTime, required, age, id);
    if (this->preemptTime > 0) {
        time -= this->preemptTime;
        preemptTime = 0;
        if (time < 0) {
            preemptTime -= time;
        }
    }
    // debug_print("halfway thru serving for %Lf. Preempt=%Lf, req=%Lf, age=%Lf, id=%d\n", time, preemptTime, required, age, id);

    if (time > 0) {
        this->age += time;
        this->required -= time;
    }

    debug_print("id %d: %Lf remaining, done=%d\n", id, required, this->done());
    return;
}

real Job::getRequired() const {
    return this->required;
}

real Job::getArrival() const {
    return this->arrivalTime;
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
