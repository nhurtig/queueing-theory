#include "distribution.h"
#include <stdio.h>
#include "queue_standard.h"
#include "dataStore.h"
#include "job.h"

int main(int argc, char **argv) {
    seed_rand(44);

    ExponentialDistribution dist = ExponentialDistribution(4.0);

    Job job = Job(&dist, 7.3, 3.9);

    DataStore store;

    store.addJob(job, 34);

    printf("Distribution sample: %Lf\n", dist.sample());
    printf("Time til interrupt: %Lf\n", job.nextInterrupt());
    printf("Job arrival time: %Lf\n", job.arrivalTime);
    return 0;
}
