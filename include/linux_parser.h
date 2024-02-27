#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// my helpers
typedef std::map<int, std::string> mpIntStr_t;
typedef std::map<std::string, std::string> mpStrStr_t;
typedef std::map<int, int> mpIntInt_t;
typedef std::vector<int> vInt_t;
typedef std::vector<std::string> vStr_t;
mpStrStr_t UtilMap1(std::string fname, char delim);
std::string UtilGetVal1(std::string fname, char delim, std::string key);
std::string UtilGetVal2(std::string fname, char delim, std::string key);
vStr_t UtilParseStr2Vec(std::string s, char delim);
mpIntStr_t GetMapUidUsrName();
mpIntInt_t GetMapPidUid(vInt_t pids);
mpIntStr_t GetMapPidUsrName(vInt_t pids, mpIntInt_t mUid, mpIntStr_t mUidUsr);
// maps for processing proc
// mpIntStr_t mapUidUsrName = {};
// extern mpIntInt_t mapPidUid;x
// extern mpIntStr_t mapPidUser;
// void GetLinuxParseMaps(vInt_t pids);

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_, 
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
long Ram(int pid);
std::string Uid(int pid, mpIntInt_t const& mpPidUid);
std::string User(int pid, mpIntStr_t const& mpPidUsr);
float ProcCpuUtil(int pid, long systemUptime);
long int UpTime(int pid,  long systemUptime);
int getUid(int pid);

};  // namespace LinuxParser

#endif