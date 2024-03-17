#ifndef SYSTEM_H
#define SYSTEM_H
#include "queue_standard.h"
#include "dataStore.h"
#include "policy.h"
#include "policyManager.h"
#include "stream.h"
#include "job.h"
#include <string>
#include <map>

class System {
public:
    System(Stream *stream, Policy *policy);
    void runFor(real ignore_time, real record_time);
    std::list<DeadJob> getData();
    void toCSV(std::string fname);
private:
    Stream *stream;
    real time;
    DataStore data;
    PolicyManager policyManager;
    unsigned int k;
    void run(real time, bool record);
    void finishQueuedJobs();
    void recordUnfinishedJobs();
    void runStep(bool record, bool allowArrivals);
};

#endif
