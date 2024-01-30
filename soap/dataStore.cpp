#include "dataStore.h"
#include <fstream>
#include <iomanip>

DataStore::DataStore() {}

std::list<DeadJob> DataStore::getJobs() {
    return this->jobs;
}

std::list<DeadJob> DataStore::dumpJobs() {
    std::list<DeadJob> jobs = this->jobs;
    this->jobs = std::list<DeadJob>();
    return jobs;
}

void DataStore::addJob(Job *job, real finishTime) {
    this->jobs.push_back(DeadJob(job, finishTime));
}

void DataStore::toCSV(std::string fname) {
    std::ofstream file;
    file.open(fname);

    // file << std::scientific << std::setprecision(15);

    file << DeadJob::header;

    for (auto& job : this->jobs) {
        job.toCSV(&file);
    }

    file.close();
}
