#ifndef DATASTORE_H
#define DATASTORE_H
#include "queue_standard.h"
#include "job.h"
#include <list>
#include <string>

class DataStore {
public:
    DataStore();
    std::list<DeadJob> getJobs();
    std::list<DeadJob> dumpJobs();
    void toCSV(std::string fname);
    void addJob(Job*, real);
private:
    std::list<DeadJob> jobs;
};

#endif
