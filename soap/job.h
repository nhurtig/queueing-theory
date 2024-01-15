#ifndef JOB_H
#define JOB_H
#include "queue_standard.h"
#include <string>
#include <fstream>

class JobInterface {
public:
    virtual real nextInterrupt() const = 0;
    virtual real getRequired() const = 0;
    bool done() const;
    virtual void serve(real time) = 0;
    virtual void show() const = 0;

    struct HashFunction {
        std::size_t operator()(const JobInterface& job) const;
    };
    bool operator==(const JobInterface& other) const;
    virtual unsigned int getID() const = 0;
};

class Job : public JobInterface {
public:
    static unsigned int nextID;
    Job(real age, real required);
    real age;
    real arrivalTime;
    unsigned int id;
    real nextInterrupt() const;
    real getRequired() const;
    void serve(real time);
    void show() const;

    unsigned int getID() const;
private:
    real required;
};

class DeadJob {
public:
    DeadJob(Job*, real);
    void toCSV(std::ofstream *stream);
    real serviceReq;
    real arrivalTime;
    real finishTime;
    unsigned int id;
    static std::string sep;
    static std::string lineSep;
    static std::string header;
};

#endif
