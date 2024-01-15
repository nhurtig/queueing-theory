#include "distribution.h"
#include <stdio.h>
#include "queue_standard.h"
#include "dataStore.h"
#include "job.h"
#include "stream.h"
#include "system.h"
#include "policy.h"

int main(int argc, char **argv) {
    seed_rand(44);

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


    unsigned int k = 5;
    FCFSPolicy policy;
    ExponentialDistribution in(0.99);

    std::vector<real> vals { 1.0 };
    std::vector<real> probs { 1.0 };
    DiscreteDistribution serv(vals, probs);
    SingleIndepStream stream(&in, &serv);
    System system(&stream, &policy, k);

    printf("Experiment start\n");
    system.runFor(100, 10000);
    printf("Experiment end\n");

    system.toCSV("results/out.csv");

    return 0;
}
