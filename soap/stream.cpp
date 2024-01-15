#include "stream.h"
#include <functional>
#include <numeric>
#include <stdexcept>

real Stream::nextInterrupt() {
    return this->timeLeft;
}

void Stream::serve(real time) {
    this->timeLeft -= time;
}

FiniteIndepStream::FiniteIndepStream(Distribution *inDist, std::vector<Distribution*> dists, std::vector<real> probs) {
    real sum = std::accumulate(probs.begin(), probs.end(), 0.0);
    std::for_each(probs.begin(), probs.end(), [sum](real x){return x/sum;});

    this->inDist = inDist;
    this->dists = dists;
    this->probs = probs;
    this->classSize = probs.size();
    if (this->classSize != dists.size()) {
        throw std::invalid_argument("|dists| != |probs|");
    }
    this->reset();
}

Job FiniteIndepStream::popJob(real time) {
    reset();
    real x = rand_real();
    for (jclass i = 0; i < this->classSize; i++) {
        x -= probs[i];
        if (x < 0) {
            return Job(time, dists[i]);
        }
    }
    throw std::out_of_range("Did probs not add to 1?");
}

void IndepStream::reset() {
    this->timeLeft = this->inDist->sample();
}

SingleIndepStream::SingleIndepStream(Distribution *inDist, Distribution *dist) {
    this->inDist = inDist;
    this->dist = dist;
    this->reset();
}

Job SingleIndepStream::popJob(real time) {
    reset();
    return Job(time, dist);
}

bool SingleIndepStream::hasJob() {
    return this->timeLeft <= 0;
}
