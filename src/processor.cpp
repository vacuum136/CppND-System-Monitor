#include <numeric>
#include "processor.h"
#include "linux_parser.h"

//Constructor
Processor::Processor():cpu_cores_(LinuxParser::CpuCores()){}

// ADDED: Update cpu info
Processor& Processor::Update(){
    unsigned long long idle_pre, total_pre, idle, total, idle_delta, total_delta; 
    //previous cpu statistics
    idle_pre = processor_stat_[LinuxParser::kIdle_] + processor_stat_[LinuxParser::kIOwait_];
    total_pre = std::accumulate(processor_stat_.begin(), processor_stat_.end()-2, 0LL);
    //read and store the actual cpu info 
    auto it = processor_stat_.begin();
    for(auto& i:LinuxParser::CpuUtilization()){
        *it++ = std::stoll(i);
    }
    //actual cpu statistics
    idle = processor_stat_[LinuxParser::kIdle_] + processor_stat_[LinuxParser::kIOwait_];
    total = std::accumulate(processor_stat_.begin(), processor_stat_.end()-2, 0LL);
    // differentiate actual and previous cpu statistics
    idle_delta = idle - idle_pre;
    total_delta = total - total_pre;
    // calculate cpu ultilization
    cpu_ultilization_ = (float)(total_delta - idle_delta)/(float)total_delta;
    return *this;
}

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { return cpu_ultilization_; }