#include "distribution.h"
#include <stdio.h>
#include "queue_standard.h"

int main(int argc, char **argv) {
    ExponentialDistribution x = ExponentialDistribution(4.0);

    seed_rand(43);

    printf("%Lf\n", x.sample());
    return 0;
}
