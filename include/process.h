#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);     //Constructor with specified pid
  int Pid();                               // TODO: See src/process.cpp
  std::string User() const;                      // TODO: See src/process.cpp
  std::string Command() const;                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  void CpuUtilization(const Process& pre); // Calculate current cpu utilization
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  bool operator>(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  unsigned int pid_;  //ADDED: const class member pid
  std::string user_;
  std::string cmdline_;
  std::string ram_{};
  long active_jiffies_{0};
  long total_jiffies_{0};
  float cpu_utilization_{0};
};

#endif