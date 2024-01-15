#ifndef JOB_H
#define JOB_H
#include "queue_standard.h"
#include "distribution.h"
#include <string>
#include <fstream>
#include <cstddef>

class JobInterface {
public:
    virtual real nextInterrupt() const = 0;
    virtual real getRequired() const = 0;
    bool done() const;
    virtual void serve(real time) = 0;

    struct HashFunction {
        std::size_t operator()(const JobInterface& job) const;
    };
    bool operator==(const JobInterface& other) const;
    virtual unsigned int getID() const = 0;
};

class Job : public JobInterface {
public:
    static unsigned int nextID;
    Job(real, Distribution*);
    real age;
    real arrivalTime;
    unsigned int id;
    real nextInterrupt() const;
    real getRequired() const;
    void serve(real time);

    unsigned int getID() const;
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

class IndexedJob : public JobInterface {
public:
    IndexedJob(real rank, Job job);
    real rank;
    Job job;

    real nextInterrupt() const;
    real getRequired() const;
    void serve(real time);

    bool operator<(const IndexedJob& other) const;
    struct ReverseComparator {
        bool operator()(const IndexedJob& lhs, const IndexedJob &rhs) const;
    };
    unsigned int getID() const;
};

#endif
