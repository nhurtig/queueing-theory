#ifndef SYSTEM_H
#define SYSTEM_H
#include "queue_standard.h"
#include "dataStore.h"
#include "policy.h"
#include "stream.h"
#include "job.h"
#include <string>
#include <map>

class System {
public:
    System(Stream *stream, Policy *policy, unsigned int k);
    void runFor(real ignore_time, real record_time);
    std::list<DeadJob> getData();
    void toCSV(std::string fname);
private:
    std::map<unsigned int, Job> jobs;
    Stream *stream;
    Policy *policy;
    unsigned int k;
    real time;
    DataStore data;
    void run(real time, bool record);
    void serveJobs(std::vector<unsigned int> toRun, bool record);
};

#endif
