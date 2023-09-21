#ifndef DATASTORE_H
#define DATASTORE_H
#include "queue_standard.h"
#include "job.h"
#include <list>

class DataStore {
public:
    DataStore();
    std::list<DeadJob> getJobs();
    std::list<DeadJob> dumpJobs();
    void addJob(Job, real);
private:
    std::list<DeadJob> jobs;
};

#endif
