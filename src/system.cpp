#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using namespace LinuxParser;
// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vInt_t   pids = LinuxParser::Pids();
  // TODO add common for processes props into system

  mpIntStr_t mapUidUsrName = LinuxParser::GetMapUidUsrName();
  mpIntInt_t mapPidUid = LinuxParser::GetMapPidUid(pids);
  mpIntStr_t mapPidUser = LinuxParser::GetMapPidUsrName(
      pids, mapPidUid, mapUidUsrName);  // maps pid->UserName

  processes_.clear();

  for (int pid : pids) {
    Process p = Process(pid, mapPidUser, mapPidUid, _systemUptime);

    processes_.push_back(p);
  }

  sort(processes_.begin(),processes_.end());

  return processes_;
}



// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
  string kern = LinuxParser::Kernel();
  return kern;
}

// DONE Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE : Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { 
  _systemUptime=LinuxParser::UpTime();
  return _systemUptime; }