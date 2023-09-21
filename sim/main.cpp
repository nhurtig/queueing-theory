#include "distribution.h"
#include <stdio.h>
#include "queue_standard.h"
#include "job.h"

int main(int argc, char **argv) {
    seed_rand(44);

    ExponentialDistribution dist = ExponentialDistribution(4.0);

    Job job = Job(&dist, 7.3, 3.9);

    printf("Distribution sample: %Lf\n", dist.sample());
    printf("Job age: %Lf\n", job.age);
    printf("Job required: %Lf\n", job.required);
    return 0;
}
