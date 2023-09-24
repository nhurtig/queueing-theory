#ifndef JOB_H
#define JOB_H
#include "queue_standard.h"
#include "distribution.h"
#include <string>
#include <fstream>

class Job {
public:
    static unsigned int nextID;
    Job(Distribution*, real, jclass);
    Distribution *dist;
    real age;
    real arrivalTime;
    jclass job_class;
    unsigned int id;
    real nextInterrupt();
    real getRequired();
    void serve(real time);
private:
    real required;
};

class DeadJob {
public:
    DeadJob(Job, real);
    void toCSV(std::ofstream *stream);
    Distribution *dist;
    real required;
    real arrivalTime;
    real finishTime;
    jclass job_class;
    static std::string sep;
    static std::string lineSep;
    static std::string header;
};

struct JobHash {
    size_t operator() (const Job& j) const {
        return j.id;
    }
};

class IndexedJob {
public:
    IndexedJob(real index, unsigned int id);
    real index;
    unsigned int id;
};

bool operator==(const Job& lhs, const Job& rhs);

#endif
