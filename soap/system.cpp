#include "system.h"
#include <algorithm>

System::System(Stream *s, Policy *p, unsigned int k)
    : stream(s), time(0), data(), policyManager(k, p), k(k) {}

void System::runFor(real ignore_time, real record_time) {
    this->run(ignore_time, false);
    // debug_print("now recording\n");
    this->run(record_time, true);
}

void System::run(real runtime, bool record) {
    real stopTime = time + runtime;

    while (time < stopTime) {
        runStep(record);
        // debug_print("%ld jobs in queue\n", jobs.size());
    }
}

void System::runStep(bool record) {
    // find next interrupt
    real streamInterrupt = this->stream->nextInterrupt();
    real jobInterrupt = this->policyManager.nextInterrupt();
    // debug_print("stream interrupt: %Lf, job interrupt: %Lf\n", streamInterrupt, jobInterrupt);
    real timeToRun = std::min(streamInterrupt, jobInterrupt);
    // timeToRun = 0.01;

    // serve jobs, stream
    this->policyManager.serve(timeToRun);
    this->stream->serve(timeToRun);

    // manage side effects
    time += timeToRun;

    while(this->policyManager.hasJob()) {
        // debug_print("collect dead job\n");
        Job job = this->policyManager.getJob();
        if (record) {
            data.addJob(&job, time);
        }
    }

    while(stream->hasJob()) {
        this->policyManager.addJob(stream->popJob(time));
    }
}

std::list<DeadJob> System::getData() {
    return this->data.getJobs();
}

void System::toCSV(std::string fname) {
    this->data.toCSV(fname);
}
