#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "system.h"
#include "process.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

//ADDED: update system information
void LinuxParser::SystemUpdate(System& system){
    // read /proc/stat
    string line, key, running_processes, total_processes;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if(stream.is_open()){
        while(std::getline(stream,line) && (total_processes.empty() || running_processes.empty())){
            std::istringstream linestream(line);
            while(linestream >> key){
                if(key == "processes"){
                    linestream >> total_processes;    
                    break;
                }
                if(key == "procs_running"){
                    linestream >> running_processes;
                    break;
                }
            }
        }
    }
    system.total_processes_ = std::stoi(total_processes);
    system.running_processes_ = std::stoi(running_processes);
    system.mem_ultilization_ = LinuxParser::MemoryUtilization();
    system.uptime_ = LinuxParser::UpTime();
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key, mem_total, mem_free; 
  
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while(std::getline(stream,line) && (mem_total.empty() || mem_free.empty())){
      std::istringstream linestream(line);
      while(linestream >> key){
        if(key == "MemTotal:"){
          linestream >> mem_total;
          break;
        }
        if(key == "MemFree:"){
          linestream >> mem_free;
          break;
        }
      }
    }
  }
  return (1.0 - std::stof(mem_free)/std::stof(mem_total));
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line, uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return std::stol(uptime);
}

//ADDED: check the number of cpu cores
int LinuxParser::CpuCores(){
  std::string line, key;
  int n;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key){
        if(key.substr(0,3) == "cpu"){
          if(key.size() > 3){
            n = std::stoi(key.substr(3));
          }
          break;
        }
        return n+1;
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, key;
  vector<string> res(10);
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key){
        if (key == "cpu"){
          for( int i = 0; i < 10; ++i)
            linestream >> res[i];
          return res;
        }
      }
    }
  }
  return {};
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, total_processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line) && total_processes.empty()){
      std::istringstream linestream(line);
      while(linestream >> key){
        if(key == "processes"){
          linestream >> total_processes;
          break;
        }
      }
    }
  }
  return std::stoi(total_processes);
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, running_procs;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line) && running_procs.empty()){
      std::istringstream linestream(line);
      while(linestream >> key){
        if(key == "procs_running"){
          linestream >> running_procs;
          break;
        }
      }
    }
  }
  return std::stoi(running_procs);
}
//ADDED: update process information
void LinuxParser::ProcessUpdate(Process& process){
  string line, key, utime, stime, cutime, cstime, starttime;
  long total_time;
  std::ifstream stream(kProcDirectory + std::to_string(process.pid_) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 1; i <= 22; ++i){
      linestream >> key;
      switch(i) {
        case 14:
            utime = key;
            break;
        case 15:
            stime = key;
            break;
        case 16:
            cutime = key;
            break;
        case 17:
            cstime = key;
            break;
        case 22:
            starttime = key;
            break;
      }
    }
  }
  total_time = std::stol(utime) + std::stol(stime) + std::stol(cutime) + std::stol(cstime);
  process.uptime_ = UpTime() - std::stol(starttime)/sysconf(_SC_CLK_TCK);
  process.cpu_ultilization_ = (float)(total_time/sysconf(_SC_CLK_TCK))/\
      (float)(process.uptime_);
  process.ram_ = Ram(process.pid_);
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream,line);
  }
  return line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key, ram_size;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream,line) && ram_size.empty()){
      std::istringstream linestream(line);
      while(linestream >> key){
        if(key == "VmSize:"){
          linestream >> ram_size;
          break;
        }
      }
    }
  }
  if(!ram_size.empty()){
    return std::to_string(std::stoi(ram_size) / 1024);
  }else{
    return string();
  }
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key, uid; 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream,line) && uid.empty()){
      std::istringstream linestream(line);
      while(linestream >> key){
        if(key == "Uid:"){
          linestream >> uid;
          break;
        }
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string key, uid, username, password;
  uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream,line)){
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream(line);
      while(linestream >> username >> password >> key){
        if(key == uid)
          return username;
      }
    }
  }
  return string();
}

//DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, up_clock_ticks;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 22; ++i){
      linestream >> up_clock_ticks;
    }
  }
  return std::stol(up_clock_ticks) / sysconf(_SC_CLK_TCK);
}
