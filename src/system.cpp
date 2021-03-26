#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

//System construtor
System::System():kernel_(LinuxParser::Kernel()), 
    operating_system_(LinuxParser::OperatingSystem()){}

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    //store previous processes into a set
    std::set<Process> processes_pre(processes_.begin(),processes_.end());
    //get actual processes
    std::vector<int> pids_current = LinuxParser::Pids();
    //clear and ready for update
    processes_.clear();
    //check each actual pid in previous set
    //if found previous process, then update the cpu utilization
    //if process is new, unnecessary to update the cpu utilization 
    //final, add the process into the vector of processes
    for(int pid:pids_current){
        Process tmp(pid);
        if(!tmp.Ram().empty()){
            auto got = processes_pre.find(tmp);
            if(got != processes_pre.end()){
                tmp.CpuUtilization(*got);
            }
            processes_.push_back(tmp);
        }
    }
    // descending sort the vector based on cpu utilization
    std::sort(processes_.begin(),processes_.end(),[](Process& a, Process& b){ return a > b;});
    return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return operating_system_; }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return (long)LinuxParser::UpTime(); }