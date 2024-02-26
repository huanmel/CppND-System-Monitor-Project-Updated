#ifndef PROC_ABSTRACT_H
#define PROC_ABSTRACT_H

#include <string>

class Proc_abstract {
public:
virtual long UpTime() const =0;
virtual std::string Ram() const =0;
virtual long RamInt() const =0;
virtual float CpuUtilization() const =0;
};


#endif