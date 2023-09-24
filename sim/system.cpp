#include "system.h"
#include <algorithm>

/*class System {
public:
    System(Stream stream, Policy policy, unsigned int k);
    void runFor(real ignore_time, real record_time);
    std::list<DeadJob> getData();
private:
    Stream stream;
    Policy policy;
    unsigned int k;
    real time;
    DataStore data;
};*/

System::System(Stream *s, Policy *p, unsigned int k) {
    this->stream = s;
    this->policy = p;
    this->k = k;
    this->time = 0;
    this->data = DataStore();
    this->jobs = std::unordered_set<Job, JobHash>();
}

void System::runFor(real ignore_time, real record_time) {
    this->run(ignore_time, false);
    this->run(record_time, true);
}

void System::run(real runtime, bool record) {
    real stopTime = time + runtime;

    while (time < stopTime) {
        serveJobs(policy->choose(jobs, k), record);
    }
}

void System::serveJobs(std::unordered_set<Job, JobHash> toRun, bool record) {
    // find next interrupt
    Job closestJob = *std::min_element(toRun.begin(), toRun.end(), [](Job a, Job b){return a.nextInterrupt() < b.nextInterrupt();});
    real timeTilJob = closestJob.required*k;
    real timeTilArrive = stream->nextInterrupt();

    real timeToRun = std::min(timeTilJob, timeTilArrive);
    // serve jobs, stream
    std::for_each(toRun.begin(), toRun.end(), [timeToRun, this](Job j){j.serve(timeToRun/k);});
    stream->serve(timeToRun);

    // manage side effects
    time += timeToRun;
    if (timeTilJob < timeTilArrive) { // job completed
        jobs.erase(closestJob);
        if (record) {
            data.addJob(closestJob, time);
        }
    } else { // job arrived
        jobs.insert(stream->popJob(time));
    }

}

std::list<DeadJob> System::getData() {
    return this->data.getJobs();
}

void System::toCSV(std::string fname) {
    this->data.toCSV(fname);
}
