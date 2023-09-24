#include "system.h"

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
}

void System::runFor(real ignore_time, real record_time) {
}

std::list<DeadJob> System::getData() {
    return this->data.getJobs();
}

void System::toCSV(std::string fname) {
    this->data.toCSV(fname);
}
