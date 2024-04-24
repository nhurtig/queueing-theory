#include "preemptPolicy.h"

GoergAlphaPreemptPolicy::GoergAlphaPreemptPolicy(real alpha): alpha{alpha} {}

bool GoergAlphaPreemptPolicy::preempt(IndexedJob y, std::priority_queue<IndexedJob> queue) const {
    if (queue.empty()) {
        return false;
    }
    IndexedJob z1 = queue.top();
    return ( y.getRequired() + y.getPreempt() ) >
        ( z1.getRequired() + z1.getPreempt() + alpha );
}

SingleGammaHyperDistributionPreemptPolicy::SingleGammaHyperDistributionPreemptPolicy(real gamma, real lambda, real cb)
    : gamma{gamma}, lambda{lambda}, cb{cb}, p{calculateLoad()}, w{(cb-1)/(cb+1)} {}

bool SingleGammaHyperDistributionPreemptPolicy::preempt(IndexedJob y,
    std::priority_queue<IndexedJob> queue) const {
    // unsigned int N = queue.size() + 1;
    real beta = 1 - y.getPreempt() / gamma;
    // printf("Beta=%Lf\n", beta);

    std::priority_queue<IndexedJob> q_copy = queue;
    
    unsigned int n1 = 0; // how many z's (smaller than y)
    real Z = 0; // how much work the z's are, INCLUDING GAMMA
    while (!q_copy.empty()) {
        if (q_copy.top().getRequired() < y.getRequired()) {
            n1++;
            Z += q_copy.top().getRequired() + gamma;
            q_copy.pop();
        } else {
            break;
        }
    }
    unsigned int n2 = 0; // how many x's (bigger than y)
    real X = 0;
    while (!q_copy.empty()) {
        X += q_copy.top().getRequired() + gamma;
        n2++;
        q_copy.pop();
    }
    // printf("should be equal: %d=%d with N=%d\n", n2, N - n1 - 1, N);

    real kappa = -3; // number of extra preempts. Should be >= 0
    real PSy = probabilitySLessThanY(y.getRequired());
    real pyprime = relativeLoad(y.getRequired()) + (1+kappa)*gamma*lambda*PSy;
    // printf("\n\n\n\ny=%Lf, PSy=%Lf, py=%Lf, p=%Lf\n\n\n\n\n", y.getRequired(), PSy, py, p);
    // printf("p=%Lf\n", p);

    real pprime = p + (1+kappa)*gamma*lambda;

    // ignore gamma for now
    // TODO: change NyZ back from Ny(y+Z) and same for AyZ
    // real NyZ = PSy*lambda*(Z+y.getRequired()) / (1-pyprime);
    real NyZ = PSy*lambda*(Z) / (1-pyprime);
    real NZyX = lambda * (Z + y.getRequired() + X) / (1-pprime);
    // real AyZ = pyprime * (Z + y.getRequired()) / (1 - pyprime);
    real AyZ = pyprime * (Z) / (1 - pyprime);

    // make static
    // NyZ = 0;
    // NZyX = 0;
    // AyZ = 0;

    //dynamic!
    // printf("beta=%Lf, pprime=%Lf, pyprime=%Lf, n1=%d, NyZ=%Lf, y=%Lf, n2=%d, NZyX-NyZ=%Lf, Z=%Lf, AyZ=%Lf\n",
    //     beta, pprime, pyprime, n1, NyZ, y.getRequired(), n2, NZyX-NyZ, Z, AyZ);
    real left_half = (n1 + NyZ) * (y.getRequired() + (1-beta)*gamma);
    real right_half = (1 + n2 + (NZyX - NyZ))*beta*gamma + (Z + AyZ);

    // printf("%Lf >? %Lf\n", left_half, right_half);

    return left_half > right_half;
}

real SingleGammaHyperDistributionPreemptPolicy::calculateLoad() const {
    // p = E[S]*lambda/mu
    // E[S] = pi/li for each subdist
    // E[S] = (1+w)/2 / 1+w   +     (1-w) / 2 / 1 - w
    //      = 1/2 + 1/2 = 1
    //
    // also mu = 1
    // printf("rho=%Lf\n", lambda);
    return lambda;
}

real SingleGammaHyperDistributionPreemptPolicy::relativeLoad(real y) const {
    // py = E[S * 1_{S < y}] * lambda / mu
    //
    // calculate integral 0 to y of f(x) x

    real leftHalfNum =  (-1 + (-w - 1) * y) * expl(-1*(w+1)*y);
    real rightHalfNum = (-1 + (-1 + w) * y) * expl((-1+w)*y);

    return ((leftHalfNum / 2) + 1 + (rightHalfNum / 2)) * lambda;
}

real SingleGammaHyperDistributionPreemptPolicy::probabilitySLessThanY(real y) const {
    real leftHalfNum =  (-w - 1) * expl(-1*(w+1)*y);
    real rightHalfNum = (-1 + w) * expl((-1+w)*y);

    return ((leftHalfNum / 2) + 1 + (rightHalfNum / 2));
}

// NEW KAPPA STUFF

ScalingSingleGammaHyperDistributionPreemptPolicy::ScalingSingleGammaHyperDistributionPreemptPolicy(real gamma, real lambda, real cb, real kappa)
    : gamma{gamma}, lambda{lambda}, kappa{kappa}, cb{cb}, p{calculateLoad()}, w{(cb-1)/(cb+1)} {}

bool ScalingSingleGammaHyperDistributionPreemptPolicy::preempt(IndexedJob y,
    std::priority_queue<IndexedJob> queue) const {
    // unsigned int N = queue.size() + 1;
    real beta = 1 - y.getPreempt() / gamma;
    // printf("Beta=%Lf\n", beta);

    std::priority_queue<IndexedJob> q_copy = queue;
    
    unsigned int n1 = 0; // how many z's (smaller than y)
    real Z = 0; // how much work the z's are, INCLUDING GAMMA
    while (!q_copy.empty()) {
        if (q_copy.top().getRequired() < y.getRequired()) {
            n1++;
            Z += q_copy.top().getRequired() + gamma;
            q_copy.pop();
        } else {
            break;
        }
    }
    unsigned int n2 = 0; // how many x's (bigger than y)
    real X = 0;
    while (!q_copy.empty()) {
        X += q_copy.top().getRequired() + gamma;
        n2++;
        q_copy.pop();
    }
    // printf("should be equal: %d=%d with N=%d\n", n2, N - n1 - 1, N);

    real PSy = probabilitySLessThanY(y.getRequired());
    real pyprime = relativeLoad(y.getRequired()) + gamma*lambda*PSy;
    // printf("\n\n\n\ny=%Lf, PSy=%Lf, py=%Lf, p=%Lf\n\n\n\n\n", y.getRequired(), PSy, py, p);
    // printf("p=%Lf\n", p);

    real pprime = p + gamma*lambda;

    // ignore gamma for now
    // TODO: change NyZ back from Ny(y+Z) and same for AyZ
    // real NyZ = PSy*lambda*(Z+y.getRequired()) / (1-pyprime);
    real NyZ = PSy*lambda*(Z) / (1-pyprime);
    real NZyX = lambda * (Z + y.getRequired() + X) / (1-pprime);
    // real AyZ = pyprime * (Z + y.getRequired()) / (1 - pyprime);
    real AyZ = pyprime * (Z) / (1 - pyprime);

    // make static
    // NyZ = 0;
    // NZyX = 0;
    // AyZ = 0;

    //dynamic!
    // printf("beta=%Lf, pprime=%Lf, pyprime=%Lf, n1=%d, NyZ=%Lf, y=%Lf, n2=%d, NZyX-NyZ=%Lf, Z=%Lf, AyZ=%Lf\n",
    //     beta, pprime, pyprime, n1, NyZ, y.getRequired(), n2, NZyX-NyZ, Z, AyZ);
    real left_half = (n1 + NyZ) * (y.getRequired() + (1-beta)*gamma);
    real right_half = (1 + n2 + (NZyX - NyZ))*beta*gamma + (Z + AyZ);

    // printf("%Lf >? %Lf\n", left_half, right_half);

    return ((1+kappa)/(1-kappa)) * left_half > right_half;
}

real ScalingSingleGammaHyperDistributionPreemptPolicy::calculateLoad() const {
    // p = E[S]*lambda/mu
    // E[S] = pi/li for each subdist
    // E[S] = (1+w)/2 / 1+w   +     (1-w) / 2 / 1 - w
    //      = 1/2 + 1/2 = 1
    //
    // also mu = 1
    // printf("rho=%Lf\n", lambda);
    return lambda;
}

real ScalingSingleGammaHyperDistributionPreemptPolicy::relativeLoad(real y) const {
    // py = E[S * 1_{S < y}] * lambda / mu
    //
    // calculate integral 0 to y of f(x) x

    real leftHalfNum =  (-1 + (-w - 1) * y) * expl(-1*(w+1)*y);
    real rightHalfNum = (-1 + (-1 + w) * y) * expl((-1+w)*y);

    return ((leftHalfNum / 2) + 1 + (rightHalfNum / 2)) * lambda;
}

real ScalingSingleGammaHyperDistributionPreemptPolicy::probabilitySLessThanY(real y) const {
    real leftHalfNum =  (-w - 1) * expl(-1*(w+1)*y);
    real rightHalfNum = (-1 + w) * expl((-1+w)*y);

    return ((leftHalfNum / 2) + 1 + (rightHalfNum / 2));
}
