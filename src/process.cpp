#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, mpIntStr_t const& mpPidUsr,
                 mpIntInt_t const& mpPidUid, long sysUptime)
    : _pid{pid} {
  _pUser = LinuxParser::User(pid, mpPidUsr);
  _pUid = LinuxParser::Uid(pid, mpPidUid);
  _pCommand = LinuxParser::Command(pid);
  _pCpuutil = LinuxParser::ProcCpuUtil(pid, sysUptime);
  _pRam = LinuxParser::Ram(pid);
  _pUptime = LinuxParser::UpTime(pid, sysUptime);
}

// TODO: Return this process's ID
int Process::Pid() const { return _pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return _pCpuutil; }

// TODO: Return the command that generated this process
string Process::Command() const { return _pCommand; }

// DONE: Return this process's memory utilization
string Process::Ram() const { return to_string(_pRam); }
long Process::RamInt() const { return (_pRam); }

// TODO: Return the user (name) that generated this process
string Process::User() const { return _pUser; }

// TODO: Return the age of this process (in seconds)
long Process::UpTime() const { return _pUptime; }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return _pCpuutil > a._pCpuutil;
}
