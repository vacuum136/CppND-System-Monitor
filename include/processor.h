#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  Processor(); //Processor constructor
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    const int cpu_cores_{0};
    float cpu_utilization_{0};
    std::vector<unsigned long long> processor_stat_{std::vector<unsigned long long>(10)};
};

#endif