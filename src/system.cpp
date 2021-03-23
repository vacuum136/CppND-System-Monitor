#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

//ADDED: Construtor
System::System(){
    kernel_ = LinuxParser::Kernel();
    operating_system_ = LinuxParser::OperatingSystem();
}

 //ADDED: read and update system status
 System& System::Update(){
    LinuxParser::SystemUpdate(*this);
    return *this;
 }
// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_.Update(); }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    processes_.clear();
    std::vector<int> pids = LinuxParser::Pids();
    for(auto pid : pids){
        processes_.emplace_back(pid);
    }
    std::sort(processes_.begin(),processes_.end(),[](Process& a, Process& b){ return a > b;});
    return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return mem_ultilization_; }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return operating_system_; }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return running_processes_; }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return total_processes_; }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return uptime_; }