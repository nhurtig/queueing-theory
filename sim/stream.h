#ifndef STREAM_H
#define STREAM_H
#include "queue_standard.h"
#include "job.h"
#include <vector>

class Stream {
public:
    real nextInterrupt();
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
    FiniteIndepStream(Distribution*, std::vector<Distribution*>, std::vector<real>);
    Job popJob(real);
private:
    std::vector<Distribution*> dists;
    std::vector<real> probs;
    jclass classSize;
};

class SingleIndepStream: public IndepStream {
public:
    SingleIndepStream(Distribution*, Distribution*);
    Job popJob(real);
private:
    Distribution *dist;
};

#endif
