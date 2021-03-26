#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
float LinuxParser::UpTime() {
  string line, uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return std::stof(uptime);
}

// ADDED: check the number of cpu cores
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

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string line, key;
  long total_jiffies = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      if(linestream >> key && key == "cpu"){
        for( int i = 0; i < 8; ++i){
          linestream >> key;
          total_jiffies += std::stol(key);
        }
        break;
      }
    }
  }
  return total_jiffies;
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, key, utime, stime, cutime, cstime, starttime;
  long jiffies_pid = 0;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
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
  //here both this process itself and its children's process counted
  jiffies_pid = std::stol(utime) + std::stol(stime) + std::stol(cutime) + std::stol(cstime);
  
  //here only this process itself counted
  //jiffies_pid = std::stol(utime) + std::stol(stime);
  return jiffies_pid;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long active_jiffies = LinuxParser::Jiffies() > LinuxParser::IdleJiffies() ? (LinuxParser::Jiffies() > LinuxParser::IdleJiffies()) : 0;
  return active_jiffies;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::vector<long>cpu_states;
  auto it = cpu_states.begin();
  for(auto i : LinuxParser::CpuUtilization()){
    *it++ = std::stol(i);
  }
  long idle = cpu_states[kIdle_] + cpu_states[kIOwait_];
  return idle;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, key;
  vector<string> res(10);
  std::ifstream stream(kProcDirectory + kStatFilename);
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

// DONE: Read and return the uptime of a process
float LinuxParser::UpTime(int pid) {
  string line, starttime;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 22; ++i){
      linestream >> starttime;
    }
  }
  return LinuxParser::UpTime() - std::stof(starttime)/(float)sysconf(_SC_CLK_TCK);
}
