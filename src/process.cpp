#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

//ADDED: Constructor with specified pid
Process::Process(int pid):pid_(pid),
    user_(LinuxParser::User(pid_)),cmdline_(LinuxParser::Command(pid_))
{
    LinuxParser::ProcessUpdate(*this);
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_ultilization_; }

// DONE: Return the command that generated this process
string Process::Command() const { return cmdline_; }

// DONE: Return this process's memory utilization
string Process::Ram() { return ram_; }

// DONE: Return the user (name) that generated this process
string Process::User() const { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return this->cpu_ultilization_ < a.cpu_ultilization_;
}

// DONE: Overload the "more than" comparison operator for Process objects
bool Process::operator>(Process const& a) const {
    return this->cpu_ultilization_ > a.cpu_ultilization_;
}