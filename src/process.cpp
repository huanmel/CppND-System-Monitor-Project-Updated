#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, mpIntStr_t mpPidUsr, mpIntInt_t mpPidUid, long sysUptime): _pid{pid}
{
    
_pUser = LinuxParser::User(pid, mpPidUsr);
_pUid = LinuxParser::Uid(pid, mpPidUid);
_pCommand = LinuxParser::Command(pid);
_pCpuutil = LinuxParser::ProcCpuUtil(pid, sysUptime);
_pRam = LinuxParser::Ram(pid);
_pUptime = LinuxParser::UpTime(pid);
}

// TODO: Return this process's ID
int Process::Pid() {  return _pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return _pCpuutil; }

// TODO: Return the command that generated this process
string Process::Command() { return _pCommand; }

// TODO: Return this process's memory utilization
string Process::Ram() { return to_string(_pRam); }

// TODO: Return the user (name) that generated this process
string Process::User() { return _pUser; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return _pUptime; }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
     return _pCpuutil>a._pCpuutil; }

// bool Process::compareProcMem(Process const& a, Process const& b) const
//   {
//     return a._pRam>b._pRam; 
//   }
