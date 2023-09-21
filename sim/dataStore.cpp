#include "dataStore.h"

DataStore::DataStore() {
    
}

std::list<DeadJob> DataStore::getJobs() {
    return this->jobs;
}

std::list<DeadJob> DataStore::dumpJobs() {
    std::list<DeadJob> jobs = this->jobs;
    this->jobs = std::list<DeadJob>();
    return jobs;
}

void DataStore::addJob(Job job, real finishTime) {
    this->jobs.push_back(DeadJob(job, finishTime));
}
