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
    this->jobs = std::map<unsigned int, Job>();
}

void System::runFor(real ignore_time, real record_time) {
    this->run(ignore_time, false);
    debug_print("now recording\n");
    this->run(record_time, true);
}

void System::run(real runtime, bool record) {
    real stopTime = time + runtime;

    while (time < stopTime) {
        serveJobs(policy->choose(jobs, k), record);
        debug_print("%ld jobs in queue\n", jobs.size());
    }
}

void System::serveJobs(std::vector<unsigned int> toRun, bool record) {
    // find next interrupt
    real timeTilJob = infinity;
    auto closestJob = std::min_element(toRun.begin(), toRun.end(), [this](unsigned int id1, unsigned int id2){return jobs.find(id1)->second.nextInterrupt() < jobs.find(id2)->second.nextInterrupt();});
    if (toRun.size() > 0) {
        timeTilJob = jobs.find(*closestJob)->second.nextInterrupt()*k;
    }
    real timeTilArrive = stream->nextInterrupt();

    real timeToRun = std::min(timeTilJob, timeTilArrive);
    // serve jobs, stream
    std::for_each(toRun.begin(), toRun.end(), [timeToRun, this](unsigned int id){jobs.find(id)->second.serve(timeToRun/k);});
    stream->serve(timeToRun);

    // manage side effects
    time += timeToRun;
    if (timeTilJob < timeTilArrive) { // job completed
        if (record) {
            data.addJob(jobs.find(*closestJob)->second, time);
        }
        jobs.erase(*closestJob);
    } else { // job arrived
        Job newJob = stream->popJob(time);
        jobs.emplace(newJob.id, newJob);
    }
}

std::list<DeadJob> System::getData() {
    return this->data.getJobs();
}

void System::toCSV(std::string fname) {
    this->data.toCSV(fname);
}
