#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <filesystem>
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
namespace fs = std::filesystem;
// DONE: Return the system's CPU
Processor& System::Cpu() {
  cpu_ = Processor();
  return cpu_;
}

bool compareProcMem(Process const& a, Process const& b) {
  return a.RamInt() > b.RamInt();
}

bool compareProcCpu(Process const& a, Process const& b) {
  return a.CpuUtilization() > b.CpuUtilization();
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vInt_t pids = LinuxParser::Pids();
  // TODO add common for processes props into system

  mpIntStr_t mapUidUsrName = LinuxParser::GetMapUidUsrName();
  mpIntInt_t mapPidUid = LinuxParser::GetMapPidUid(pids);
  mpIntStr_t mapPidUser = LinuxParser::GetMapPidUsrName(
      pids, mapPidUid, mapUidUsrName);  // maps pid->UserName

  processes_.clear();
  std::string fpath;

  for (int pid : pids) {
    fpath = kProcDirectory + std::to_string(pid) + kStatusFilename;
    if (fs::exists(fpath))  // skip non existing pid directory
    {
      Process p = Process(pid, mapPidUser, mapPidUid, _systemUptime);

      processes_.push_back(p);
    }
  }

  sort(processes_.begin(), processes_.end(), compareProcMem);

  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
  _kernel = LinuxParser::Kernel();
  return _kernel;
}

// DONE Return the system's memory utilization
float System::MemoryUtilization() {
  _memutil = LinuxParser::MemoryUtilization();
  return _memutil;
}

// DONE : Return the operating system name
std::string System::OperatingSystem() {
  _operatingsys = LinuxParser::OperatingSystem();
  return _operatingsys;
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() {
  _runningprocess = LinuxParser::RunningProcesses();
  return _runningprocess;
}

// DONE: Return the total number of processes on the system
int System::TotalProcesses() {
  _totalproc = LinuxParser::TotalProcesses();
  return _totalproc;
}

// DONE: Return the number of seconds since the system started running
long int System::UpTime() {
  _systemUptime = LinuxParser::UpTime();
  return _systemUptime;
}