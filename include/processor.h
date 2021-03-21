#ifndef PROCESSOR_H
#define PROCESSOR_H

#include"linux_parser.h"

class Processor {
public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
private:
    unsigned long long int user_{0};
    unsigned long long int nice_{0};
    unsigned long long int system_{0};
    unsigned long long int idle_{0};
    unsigned long long int iowait_{0};
    unsigned long long int irq_{0};
    unsigned long long int soft_irq_{0};
    unsigned long long int steal_{0};
    unsigned long long int guest_{0};
    unsigned long long int guest_nice_{0};
};

#endif