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
#ifndef STREAM_H
#define STREAM_H
#include "queue_standard.h"
#include "job.h"
#include <vector>

class Stream {
public:
    real nextInterrupt();
    void serve(real time);
    virtual Job popJob(real) = 0;
    virtual void reset() = 0;
protected:
    real timeLeft;
};

class IndepStream: public Stream {
public:
    void reset();
protected:
    Distribution *inDist;
};

class FiniteIndepStream: public IndepStream {
public:
    FiniteIndepStream(Distribution* in, std::vector<Distribution*> serv, std::vector<real> probs);
    Job popJob(real);
private:
    std::vector<Distribution*> dists;
    std::vector<real> probs;
    jclass classSize;
};

class SingleIndepStream: public IndepStream {
public:
    SingleIndepStream(Distribution* in, Distribution* serv);
    Job popJob(real);
private:
    Distribution *dist;
};

#endif
#ifndef LIMITEDSET_H
#define LIMITEDSET_H
#include "queue_standard.h"
#include "job.h"
#include <vector>

class LimitedSet {
public:
    LimitedSet(unsigned int k);
    void insert(IndexedJob job);
    std::vector<unsigned int> toVector();
    ~LimitedSet();
private:
    IndexedJob* arr;
    unsigned int k;
    unsigned int size;
    void insertInternal(IndexedJob job, unsigned int i);
};

#endif
#ifndef QUEUE_STANDARD_H
#define QUEUE_STANDARD_H
#include <cmath>
#include <stdio.h>
#include <cfloat>

#define DEBUG_MODE
#ifdef DEBUG_MODE
#define debug_print(...) fprintf(stderr, __VA_ARGS__)
#endif
#ifndef DEBUG_MODE
#define debug_print(...)
#endif

typedef long double real;
#define infinity LDBL_MAX;
typedef unsigned int jclass;
#define ln logl

#endif
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
    void addJob(Job, real);
private:
    std::list<DeadJob> jobs;
};

#endif
#ifndef POLICY_H
#define POLICY_H
#include "queue_standard.h"
#include "job.h"
#include <vector>
#include <map>

class Policy {
public:
    virtual std::vector<unsigned int> choose(std::map<unsigned int, Job> jobs, unsigned int k) = 0;
};

class IndexPolicy: public Policy {
public:
    std::vector<unsigned int> choose(std::map<unsigned int, Job> jobs, unsigned int k);
private:
    virtual real getIndex(Job job) = 0;
};

class FCFSPolicy: public IndexPolicy {
private:
    real getIndex(Job job);
};

bool compareJobs(IndexedJob a, IndexedJob b);

#endif
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
#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H
#include "queue_standard.h"
#include <vector>

real rand_real();

void seed_rand(int);

class Distribution {
public:
    // virtual real pdf(real) = 0;
    virtual real cdf(real) = 0;
    virtual real sample() = 0;
};

class ExponentialDistribution: public Distribution {
    public:
        ExponentialDistribution(real);
        // real pdf(real);
        real cdf(real);
        real sample();
    private:
        real lambda;
};

class DiscreteDistribution: public Distribution {
    public:
        DiscreteDistribution(std::vector<real> vals, std::vector<real> probs);
        real cdf(real);
        real sample();
    private:
        unsigned int size;
        std::vector<real> vals;
        std::vector<real> probs;
};

class DegenerateDistribution: public Distribution {
    public:
        DegenerateDistribution(real x);
        real cdf(real);
        real sample();
    private:
        real x;
};

#endif
