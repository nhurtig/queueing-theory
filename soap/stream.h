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
    virtual bool hasJob() = 0;
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
    bool hasJob();
private:
    Distribution *dist;
};

#endif
