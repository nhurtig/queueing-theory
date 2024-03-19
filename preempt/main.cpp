#include "distribution.h"
#include <stdio.h>
#include <sstream>
#include "queue_standard.h"
#include "dataStore.h"
#include "job.h"
#include "stream.h"
#include "system.h"
#include "policy.h"

int main(int argc, char **argv) {
/*
    ExponentialDistribution dist = ExponentialDistribution(4.0);

    Job job = Job(7.4, &dist);

    DataStore store;

    store.addJob(&job, 34);

    printf("Store size: %ld\n", store.getJobs().size());
    store.dumpJobs();
    printf("Store size: %ld\n", store.getJobs().size());
    printf("Distribution sample: %Lf\n", dist.sample());
    printf("Time til interrupt: %Lf\n", job.nextInterrupt());
    printf("Job arrival time: %Lf\n", job.arrivalTime);


    ExponentialDistribution in(0.001);
    ExponentialDistribution serv(3);
    SingleIndepStream stream(&in, &serv);

    printf("Stream til interrupt: %Lf\n", stream.nextInterrupt());
    printf("Stream's job pop til interrupt: %Lf\n", stream.popJob(3).nextInterrupt());
    printf("Stream til interrupt: %Lf\n", stream.nextInterrupt());
    printf("Stream's job pop til interrupt: %Lf\n", stream.popJob(3).nextInterrupt());
    printf("Stream til interrupt: %Lf\n", stream.nextInterrupt());

    FCFSPolicy pol;
    PolicyManager mgr(3, &pol);

    mgr.addJob(job);
    mgr.nextInterrupt();
    mgr.serve(3);
*/

/*
    // LOG experiment (gittins vs FCFS in turnaround for WiQ presentation)
    // server characteristics
    unsigned int k = 3;

    unsigned int RUN_TIME = 10000;
    unsigned int TRIALS = 100;
    real MAX_N = 3; // 1-10^-3 = 0.999
    real step = 0.1;

    // input characteristics
    // used to be 1.43
    std::vector<real> vals { 20.0/27, 40.0/27, 60.0/27 };
    std::vector<real> probs { 0.8, 0.05, 0.15 }; // mean is 1
    DiscreteDistribution serv(vals, probs);
    // ExponentialDistribution serv(1.01);

    DiscreteGittinsPolicy policyGittins(vals, probs);
    FCFSPolicy policyFCFS;

    long int seed = 400000;
    for (real n = 0; n < MAX_N; n += step) {
        real load = 1.0 - powl(10.0, -n);

        for (unsigned int i = 0; i < TRIALS; i++) {
            seed_rand(seed);
            ExponentialDistribution inFCFS(load);
            SingleIndepStream streamFCFS(&inFCFS, &serv);
            System systemFCFS(&streamFCFS, &policyFCFS, k);
            systemFCFS.runFor(0, RUN_TIME);
            std::ostringstream ossFCFS;
            ossFCFS << "results/log/fcfs_" << n << "_" << load << "_" << i << ".csv";
            systemFCFS.toCSV(ossFCFS.str());

            seed_rand(seed);
            ExponentialDistribution inGittins(load);
            SingleIndepStream streamGittins(&inGittins, &serv);
            System systemGittins(&streamGittins, &policyGittins, k);
            systemGittins.runFor(0, RUN_TIME);
            std::ostringstream ossGittins;
            ossGittins << "results/log/gittins_" << n << "_" << load << "_" << i << ".csv";
            systemGittins.toCSV(ossGittins.str());
            seed++;
        }
    }
//*/

/*
    // gittins vs slowdown gittins
    // server characteristics
    std::vector<unsigned int> ks;
    ks.push_back(1);
    ks.push_back(5);
    ks.push_back(20);

    unsigned int RUN_TIME = 10000;
    unsigned int TRIALS = 100;
    real MAX_N = 3; // 1-10^-3 = 0.999
    real step = 0.1;

    // input characteristics
    // used to be 1.43
    std::vector<real> vals { 20.0/27, 40.0/27, 60.0/27 };
    // std::vector<real> vals { 1, 2, 3 };
    std::vector<real> probs { 0.8, 0.05, 0.15 }; // mean is 1
    DiscreteDistribution serv(vals, probs);
    // ExponentialDistribution serv(1.01);

    DiscreteGittinsPolicy policyTurn(vals, probs);
    DiscreteGittinsSlowdownPolicy policySlow(vals, probs);

    long int seed = 4;
    for (const auto& k : ks) {
    for (real n = 0; n < MAX_N; n += step) {
        real load = 1.0 - powl(10.0, -n);
        // real load = 1.3;

        for (unsigned int i = 0; i < TRIALS; i++) {
            seed_rand(seed);
            ExponentialDistribution inTurn(load);
            SingleIndepStream streamTurn(&inTurn, &serv);
            System systemTurn(&streamTurn, &policyTurn, k);
            systemTurn.runFor(0, RUN_TIME);
            std::ostringstream ossTurn;
            ossTurn << "results/gittcomp/turn_" << k << "_" << n << "_" << load << "_" << i << ".csv";
            systemTurn.toCSV(ossTurn.str());

            seed_rand(seed);
            ExponentialDistribution inSlow(load);
            SingleIndepStream streamSlow(&inSlow, &serv);
            System systemSlow(&streamSlow, &policySlow, k);
            systemSlow.runFor(0, RUN_TIME);
            std::ostringstream ossSlow;
            ossSlow << "results/gittcomp/slow_" << k << "_" << n << "_" << load << "_" << i << ".csv";
            systemSlow.toCSV(ossSlow.str());

            seed++;
        }
    }
    }
//*/

/* best alpha for various values of gamma, rho
unsigned int ignore = 0;
unsigned int record = 10000;
unsigned int n = 300;
unsigned int BASE_SEED = 20000;
unsigned int steps = 25;
unsigned int gamma_steps = 2;
unsigned int rho_steps = steps;
unsigned int alpha_steps = steps;
real gamma_min = 0;
real gamma_max = 0.1;
// real log_max_slack = -0.152003; // rho=0.1 to start with
// real log_min_slack = -13.2877; // rho=0.999 to end with
// real alpha_internal_min = 0; // 0 maps to 0
// real alpha_internal_max = 1; // 1 maps to infinity (FCFS) via x/(1-x)
for (unsigned int i = 200; i < n; i++) {
    for (real gamma = 0; gamma <= gamma_max; gamma += (gamma_max-gamma_min)/(gamma_steps-1)) {
        if (gamma != 0.1) {
            continue;
        }
        for (unsigned int lsi = 0; lsi < rho_steps; lsi++) {
            // real log_slack = log_max_slack - lsi*(log_max_slack-log_min_slack)/(rho_steps-1);
            // real rho = 1.0 - powl(2.0, log_slack);
            real rho = lsi*(1.0/rho_steps);
            for (unsigned int aii = 0; aii < alpha_steps; aii++) {
            // for (real alpha_internal = alpha_internal_min; alpha_internal <= alpha_internal_max; alpha_internal += (alpha_internal_max-alpha_internal_min)/(alpha_steps-1)) {
                real alpha = aii*(2.0/alpha_steps);
                real alpha_internal = 0.2; // because 0.2 < 1
                // real alpha_internal = alpha_internal_min + aii*(alpha_internal_max-alpha_internal_min)/(alpha_steps-1);
                seed_rand(n + BASE_SEED);
                Policy *policy = NULL;
                std::ostringstream name;
                name << "results/alpha_experiment/" << i << "_" << gamma << "_" << rho << "_";
                if (alpha_internal < 1) {
                    // (modified?) SRPT
                    // real alpha = alpha_internal/(1-alpha_internal);
                    policy = new SRPTPreemptPolicy(alpha);
                    name << alpha;
                } else {
                    // FCFS
                    policy = new SRPTPreemptPolicy(100000);
                    name << "inf";
                }
                name << "_.csv";
                AdditiveExponentialDistribution serv(2.0, 0.5-gamma);
                ExponentialDistribution in(rho);
                SingleIndepStream stream(&in, &serv, gamma);

                System system(&stream, policy);

                printf("i=%d, gamma=%Lf, rho=%Lf, alphaInternal=%Lf start\n", i, gamma, rho, alpha_internal);
                system.runFor(ignore, record);

                system.toCSV(name.str());

                delete policy;
            }
        }
    }
}
//*/

/* remake fig 6
real cb = 5;
real gamma = 0.1;
unsigned int seed_start = 3278;
unsigned int n = 1;
real ignore = 0;
real time = 100000;
std::vector<real> alphas({0, 0.5, 2.5, 5, 7.5, 10, 25, 100});
real rho_step = 0.01;
for (unsigned int i = 0; i < n; i++) {
    printf("step i=%d\n", i);
    for (real alpha : alphas) {
        printf("alpha=%Lf\n", alpha);
        for (real rho = rho_step; rho < 1; rho += rho_step) {
            seed_rand(seed_start+i);
            GoergHyperDistributionCB serv(cb);
            ExponentialDistribution in(rho/1.1);
            SingleIndepStream stream(&in, &serv, gamma);
            SRPTPreemptPolicy policy(alpha);
            System system(&stream, &policy);

            system.runFor(ignore, time);

            std::ostringstream name;
            name << "results/fig6/" << i << "_" << gamma << "_" << alpha << "_" << rho << "_.csv";
            system.toCSV(name.str());
        }
    }
}
*/

/* remake fig 7
real cb = 5;
real gamma = 0.1;
unsigned int seed_start = 3278;
unsigned int n = 1;
real ignore = 0;
real time = 100000;
std::vector<real> rhos({0.7, 0.75, 0.8, 0.85, 0.87, 0.89});
for (unsigned int i = 0; i < n; i++) {
    printf("step i=%d\n", i);
    for (real rho : rhos) {
        printf("rho=%Lf\n", rho);
        for (real alphalog = -2; alphalog < 3.05; alphalog += 0.1) {
            seed_rand(seed_start+i);
            GoergHyperDistributionCB serv(cb);
            ExponentialDistribution in(rho/1.0);
            SingleIndepStream stream(&in, &serv, gamma);
            real alpha = powl(10, alphalog);
            SRPTPreemptPolicy policy(alpha);
            System system(&stream, &policy);

            system.runFor(ignore, time);

            std::ostringstream name;
            name << "results/fig7/" << i << "_" << gamma << "_" << alpha << "_" << rho << "_.csv";
            system.toCSV(name.str());
        }
    }
}
//*/

//* awesome 3d plot
real cb = 5;
real gamma = 0.1;
unsigned int seed_start = 3278;
unsigned int n = 1;
real ignore = 0;
real time = 10000;
real rhostep = 0.01;
for (unsigned int i = 0; i < n; i++) { // i, gamma, rho, alpha
    printf("step i=%d\n", i);
    for (real rho = rhostep; rho < 1; rho += rhostep) {
        printf("rho=%Lf\n", rho);
        for (real alphalog = -2; alphalog < 3.05; alphalog += 0.1) {
            seed_rand(seed_start+i);
            GoergHyperDistributionCB serv(cb);
            ExponentialDistribution in(rho/1.1);
            SingleIndepStream stream(&in, &serv, gamma);
            real alpha = powl(10, alphalog);
            SRPTPreemptPolicy policy(alpha);
            System system(&stream, &policy);

            system.runFor(ignore, time);

            std::ostringstream name;
            name << "results/3dplot/" << i << "_" << gamma << "_" << alpha << "_" << rho << "_.csv";
            system.toCSV(name.str());
        }
    }
}
//*/


    /* Turnaround vs slowdown Gittins, simple example for debugging
    seed_rand(207);
    real gamma = 1.25;

    ExponentialDistribution serv(0.9);
    // DegenerateDistribution serv(0.9);
    ExponentialDistribution in(0.907598);
    // DegenerateDistribution in(1.2);

    SingleIndepStream stream(&in, &serv, gamma);

    SRPTPolicy policy;
    // DiscreteGittinsSlowdownPolicy policy(vals, probs);

    System system(&stream, &policy);

    // Job j = stream.popJob(0);
    // j.serve(15.0/17.0);
    // printf("0->%Lf\n", policy->getIndex(&j));

    printf("Experiment start\n");
    system.runFor(0, 200);
    printf("Experiment end\n");

    system.toCSV("results/outturn.csv");
    // system.toCSV("results/outslow.csv");
    //*/

    return 0;
}
