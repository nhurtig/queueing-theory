#ifndef PREEMPT_POLICY_H
#define PREEMPT_POLICY_H
#include "queue_standard.h"
#include "indexedJob.h"
#include <queue>
#include <memory>
#include "job.h"

class PreemptPolicy {
public:
    virtual bool preempt(IndexedJob y, std::priority_queue<IndexedJob> queue) const = 0;
    virtual ~PreemptPolicy() = default;
};

class GoergAlphaPreemptPolicy: public PreemptPolicy {
public:
    GoergAlphaPreemptPolicy(real alpha);
    bool preempt(IndexedJob y, std::priority_queue<IndexedJob> queue) const;
private:
    const real alpha;
};

class ScalingSingleGammaHyperDistributionPreemptPolicy: public PreemptPolicy {
public:
    ScalingSingleGammaHyperDistributionPreemptPolicy(real gamma, real lambda, real cb, real kappa);
    bool preempt(IndexedJob y, std::priority_queue<IndexedJob> queue) const;
private:
    const real gamma;
    const real lambda;
    const real kappa;
    const real cb;
    const real p; // rho
    const real w;
    real calculateLoad() const;
    real probabilitySLessThanY(real y) const;
    real relativeLoad(real y) const;
};

#endif
