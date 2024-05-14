#ifndef SCENARIOS_H
#define SCENARIOS_H
#include <vector>
#include <string>
#include "queue_standard.h"
#include "preemptPolicy.h"

void compute(std::vector<real> rhos, real time, unsigned int num_runs, unsigned int seed_start, real cv, real gamma, bool doGorg, std::vector<real> hyperparams, std::string prefix);

PreemptPolicy* makePolicy(real gamma, real lambda, real cv, real hyperparam, bool doGorg);

#endif
