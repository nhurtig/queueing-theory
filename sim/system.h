#ifndef SYSTEM_H
#define SYSTEM_H
#include "queue_standard.h"
#include "dataStore.h"
#include "policy.h"
#include "stream.h"
#include "job.h"

class System {
public:
    System(Stream *stream, Policy *policy, unsigned int k);
    void runFor(real ignore_time, real record_time);
    std::list<DeadJob> getData();
private:
    Stream *stream;
    Policy *policy;
    unsigned int k;
    real time;
    DataStore data;
};

#endif
