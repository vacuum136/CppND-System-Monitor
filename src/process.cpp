#include <unistd.h>
#include <cctype>
//#include <sstream>
#include <string>
//#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;

//Constructor with specified pid
Process::Process(int pid):pid_(pid),
    user_(LinuxParser::User(pid_)),cmdline_(LinuxParser::Command(pid_)){
        active_jiffies_ = LinuxParser::ActiveJiffies(pid_);
        total_jiffies_ = LinuxParser::Jiffies();
        cpu_utilization_ = (float)active_jiffies_/(float)total_jiffies_;
    }

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
    return cpu_utilization_;
}
// Calculation the current cpu ultilization of process
void Process::CpuUtilization(const Process& pre){
    long active_delta, total_delta;
    //delta
    active_delta = (active_jiffies_ > pre.active_jiffies_) ? (active_jiffies_-pre.active_jiffies_) : 0;
    total_delta = (total_jiffies_ > pre.total_jiffies_) ? (total_jiffies_-pre.total_jiffies_) : 0;
    
    cpu_utilization_ = (float)active_delta/(float)total_delta;
}

// DONE: Return the command that generated this process
string Process::Command() const { return cmdline_; }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() const { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return (long)LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return this->pid_ < a.pid_;
}

// DONE: Overload the "more than" comparison operator for Process objects
bool Process::operator>(Process const& a) const {
    return this->cpu_utilization_ > a.cpu_utilization_;
}