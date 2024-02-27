#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"
#include "proc_abstract.h"
using namespace LinuxParser;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process : public Proc_abstract {
 public:
  Process(int pid, mpIntStr_t const& mpPidUsr, mpIntInt_t const& mpPidUid,
          long sysUptime);
  int Pid() const;                         // DONE: See src/process.cpp
  std::string User() const;                // DONE: See src/process.cpp
  std::string Command() const;             // DONE: See src/process.cpp
  float CpuUtilization() const override;   // DONE: See src/process.cpp
  std::string Ram() const override;        // DONE: See src/process.cpp
  long RamInt() const override;            // DONE: See src/process.cpp
  long UpTime() const override;            // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp
  // bool compareProcMem(Process const& a,Process const& b) const;

  // DONE: Declare any necessary private members
 private:
  int _pid;  // actual PID number
  std::string _pUser;
  std::string _pUid;
  std::string _pCommand;
  float _pCpuutil;
  long _pRam;
  long _pUptime;
};

#endif