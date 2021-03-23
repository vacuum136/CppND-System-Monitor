#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 friend void LinuxParser::ProcessUpdate(Process& process);
 public:
  Process()=default;
  Process(int pid);     //ADDED: Constructor with specified pid
  int Pid();                               // TODO: See src/process.cpp
  std::string User() const;                      // TODO: See src/process.cpp
  std::string Command() const;                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  bool operator>(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  unsigned int pid_;  //ADDED: const class member pid
  std::string user_;
  std::string cmdline_;
  long uptime_{0};
  float cpu_ultilization_{0};
  std::string ram_{};
};

#endif