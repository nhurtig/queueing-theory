#ifndef JOB_H
#define JOB_H
#include "queue_standard.h"
#include "distribution.h"
#include <string>
#include <fstream>
#include <cstddef>

class Job {
public:
    static unsigned int nextID;
    Job(real, Distribution*);
    real age;
    real arrivalTime;
    unsigned int id;
    real nextInterrupt();
    real getRequired();
    void serve(real time);

    struct HashFunction {
        std::size_t operator()(const Job& job) const;
    };
    bool operator==(const Job& other) const;
private:
    real required;
};

class DeadJob {
public:
    DeadJob(Job*, real);
    void toCSV(std::ofstream *stream);
    real required;
    real arrivalTime;
    real finishTime;
    static std::string sep;
    static std::string lineSep;
    static std::string header;
};

class IndexedJob {
public:
    IndexedJob(real rank, Job job);
    real rank;
    Job job;

    struct HashFunction {
        std::size_t operator()(const IndexedJob& job) const;
    };
    bool operator==(const IndexedJob& other) const;
    bool operator<(const IndexedJob& other) const;
};

#endif
