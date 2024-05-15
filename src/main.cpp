#include "distribution.h"
#include <stdio.h>
#include <sstream>
#include "queue_standard.h"
#include "dataStore.h"
#include "job.h"
#include "stream.h"
#include "system.h"
#include "policy.h"
#include "scenarios.h"

int main(int argc, char **argv) {
    // set up constants
    std::vector<real> rhos_many({0.7, 0.75, 0.8, 0.85, 0.87, 0.89});
    real time = 10000;
    unsigned int num_runs = 50;
    unsigned int seed_start = 3280;
    real cv = 5; // this recreates the incorrect distribution
    real gamma = 0.1;

    // hyperparameters for each policy
    std::vector<real> alphas;
    for (real alphalog = -2; alphalog < 3.02; alphalog += 0.1) {
        alphas.push_back(powl(10, alphalog));
    }

    std::vector<real> kappas;
    real kappastep = 0.02;
    for (real kappa = -1 + kappastep; kappa < 1; kappa += kappastep) {
        kappas.push_back(kappa);
    }

    // explore a few load values across all hyperparameters
    // gorg
    compute(rhos_many, time, num_runs, seed_start, cv, gamma, true, alphas, "../results/explore_gorg/");
    // ours
    compute(rhos_many, time, num_runs, seed_start, cv, gamma, false, kappas, "../results/explore_ours/");

    // compute many results for just one load, one kappa, and many alphas
    std::vector<real> best_kappa({0.58});
    std::vector<real> one_rho({0.85});
    seed_start = 5278;
    time = 10;
    num_runs = 500;
    // gorg
    compute(one_rho, time, num_runs, seed_start, cv, gamma, true, alphas, "../results/compare_gorg/");
    // ours
    compute(one_rho, time, num_runs, seed_start, cv, gamma, false, best_kappa, "../results/compare_ours/");

    return 0;
}
