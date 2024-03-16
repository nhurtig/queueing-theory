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


    //* Turnaround vs slowdown Gittins, simple example for debugging
    seed_rand(4);
    real gamma = 0.1;

    ExponentialDistribution serv(0.9);
    // DegenerateDistribution serv(0.9);
    ExponentialDistribution in(0.99);
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
