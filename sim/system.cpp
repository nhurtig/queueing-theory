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
    debug_print("now recording\n");
    this->run(record_time, true);
}

void System::run(real runtime, bool record) {
    real stopTime = time + runtime;

    while (time < stopTime) {
        debug_print("time is %Lf\n", time);
        serveJobs(policy->choose(jobs, k), record);
    }
}

void System::serveJobs(std::vector<Job*> toRun, bool record) {
    // find next interrupt
    real timeTilJob = infinity;
    auto closestJob = std::min_element(toRun.begin(), toRun.end(), [](Job *a, Job *b){return a->nextInterrupt() < b->nextInterrupt();});
    if (toRun.size() > 0) {
        timeTilJob = (*closestJob)->required*k;
        debug_print("timeTilJob=%Lf\n", timeTilJob);
    }
    real timeTilArrive = stream->nextInterrupt();

    real timeToRun = std::min(timeTilJob, timeTilArrive);
    // serve jobs, stream
    std::for_each(toRun.begin(), toRun.end(), [timeToRun, this](Job *j){j->serve(timeToRun/k);});
    stream->serve(timeToRun);

    // manage side effects
    time += timeToRun;
    if (timeTilJob < timeTilArrive) { // job completed
        debug_print("job completed\n");
        jobs.erase(**closestJob);
        if (record) {
            data.addJob(**closestJob, time);
        }
    } else { // job arrived
        jobs.insert(stream->popJob(time));
        printf("jobs size is %ld\n", jobs.size());
    }
}

std::list<DeadJob> System::getData() {
    return this->data.getJobs();
}

void System::toCSV(std::string fname) {
    this->data.toCSV(fname);
}
