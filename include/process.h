#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
using namespace LinuxParser;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, mpIntStr_t mpPidUsr, mpIntInt_t mpPidUid, long sysUptime);
  int Pid();                               // DONE: See src/process.cpp
  std::string User();                      // DONE: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization();                  // DONE: See src/process.cpp
  std::string Ram();                       // DONE: See src/process.cpp
  long int UpTime();                       // DONE: See src/process.cpp
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
  long int _pUptime;
};

#endif