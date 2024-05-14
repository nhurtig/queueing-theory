#include "scenarios.h"
#include "distribution.h"
#include "stream.h"
#include "preemptPolicy.h"
#include "policyManager.h"
#include "system.h"
#include <sstream>

void compute(std::vector<real> rhos, real time, unsigned int num_runs, unsigned int seed_start, real cv, real gamma, bool doGorg, std::vector<real> hyperparams, std::string prefix) {
    real ignore = 0;

    for (unsigned int i = 0; i < num_runs; i++) {
        printf("step i=%d\n", i);
        for (real rho : rhos) {
            for (real hyperparam : hyperparams) {
                seed_rand(seed_start + i);
                GoergHyperDistributionCB serv(cv);
                real lambda = rho/1.0;
                ExponentialDistribution in(lambda);
                SingleIndepStream stream(&in, &serv, gamma);
                PreemptPolicy* policy = makePolicy(gamma, lambda, cv, hyperparam, doGorg);

                PolicyManagerPreempt pm(policy);
                System system(&stream, &pm);

                system.runFor(ignore, time);

                std::ostringstream name;
                name << prefix << i << "_" << doGorg << "_" << hyperparam << "_" << rho << "_.csv";
                system.toCSV(name.str());

                delete policy;
            }
        }
    }
}

PreemptPolicy* makePolicy(real gamma, real lambda, real cv, real hyperparam, bool doGorg) {
    if (doGorg) {
        return new GoergAlphaPreemptPolicy(hyperparam);
    } else {
        return new ScalingSingleGammaHyperDistributionPreemptPolicy(gamma, lambda, cv, hyperparam);
    }
}
