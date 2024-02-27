#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

//#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using namespace LinuxParser;
// #include <filesystem>
// namespace fs = std::filesystem;
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// DONE: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  /*  DIR* directory = opendir(kProcDirectory.c_str());
   struct dirent* file;
   while ((file = readdir(directory)) != nullptr) {
     // Is this a directory?
     if (file->d_type == DT_DIR) {
       // Is every character of the name a digit?
       string filename(file->d_name);
       if (std::all_of(filename.begin(), filename.end(), isdigit)) {
         int pid = stoi(filename);
         pids.push_back(pid);
       }
     }
   }
   closedir(directory); */
  std::string directoryPath = kProcDirectory;
  std::string dirname;
  // Iterate through the directory
  for (const auto& entry : fs::directory_iterator(directoryPath)) {
    // Check if the current entry is a directory
    if (fs::is_directory(entry)) {
      dirname = entry.path().filename().string();
      if (std::all_of(dirname.begin(), dirname.end(), isdigit)) {
        int pid = stoi(dirname);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::map<std::string, std::string> m =
      UtilMap1(kProcDirectory + kMeminfoFilename, ':');
  // get values and process them
  // prepare lambda helper for it
  /* auto proc_memstrval = [](std::string strval) {
    std::string output =
        std::regex_replace(strval, std::regex("([0-9]+).*"), std::string("$1"));

    int val = std::stoi(output);
    return val;
  }; */
  int memTotal = std::stoi(m["MemTotal"]);
  int memFree = std::stoi(m["MemFree"]);
  // int memAvailable=std::stoi(m["MemAvailable"]);
  int memCached=std::stoi(m["Cached"]);
  int memBuffer=std::stoi(m["Buffers"]);
  float memUtil = float(memTotal - memFree);
  memUtil = (memUtil - (memBuffer+memCached))  /(memTotal);
 
  // string value = UtilGetVal1(kProcDirectory + kMeminfoFilename, ' ',
  // "processes"); procs = std::stoi(value);

  return memUtil;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime = 0;
  long idletime = 0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime;
}

// CANCELLED: NO NEED: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// CANCELLED: NO NEED Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// CANCELLED: NO NEED: Read and return the number of active jiffies for the
// system
long LinuxParser::ActiveJiffies() { return 0; }

// CANCELLED: NO NEED: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> vstrings;
  string tmp;
  //  std::map<std::string, std::string> m = UtilMap1(fname, delim);
  string s = UtilGetVal2(kProcDirectory + kStatFilename, ' ', "cpu");
  std::stringstream ss(s);
  while (getline(ss, tmp, ' ')) {
    vstrings.push_back(tmp);
  }

  // return vstrings;
  return vstrings;
}

std::map<std::string, std::string> LinuxParser::UtilMap1(std::string fname,
                                                         char delim) {
  // parse input string into map [key]=value with predefined delimiter

  std::map<std::string, std::string> m;

  std::string key, val;
  std::ifstream iss(fname);

  while (std::getline(std::getline(iss, key, delim) >> std::ws, val))
    m[key] = val;
  return m;
}

string LinuxParser::UtilGetVal1(string fname, char delim, string key) {
  string s;
  std::map<std::string, std::string> m = UtilMap1(fname, delim);
  if (m.count(key) > 0) {
    s = (m[key]);
  } else {
    // throw std::invalid_argument("can't find key, default value would be
    // used"); throw waxrning("this is a warning");
    s = "NA";
  }
  return s;
};

string LinuxParser::UtilGetVal2(string fname, char delim, string key)
// more economic, no need in map if we use one value at the output
 {
  
  // std::string key, val;
  std::ifstream iss(fname);
  std::string lkey, val="NA";
  while (std::getline(std::getline(iss, lkey, delim) >> std::ws, val))
  { if (lkey==key) {break;}}
  
  return val;
};

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int procs = 0;
  // long idletime = 0;
  // string line;
  // string key;
  // string value;
  string value = UtilGetVal2(kProcDirectory + kStatFilename, ' ', "processes");
  procs = std::stoi(value);
  return procs;
  /*   std::map<std::string, std::string> m =
        UtilMap1(kProcDirectory + kStatFilename,' ');
        if (m.count("processes")>0)
        {  procs = std::stoi(m["processes"]);
        }
        else {
          throw std::invalid_argument("can't find key, default value would be
     used");
          // throw waxrning("this is a warning");
        } */

  /* std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // std::replace(line.begin(), line.end(), ' ', '_');
      // std::replace(line.begin(), line.end(), '=', ' ');
      // std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          std::replace(value.begin(), value.end(), '_', ' ');
          procs = std::stoi(value);
          break;
        }
      }
    }
  } */
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string value =
      UtilGetVal1(kProcDirectory + kStatFilename, ' ', "procs_running");
  int procs = std::stoi(value);
  return procs;
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string fpath;
  fpath = kProcDirectory + std::to_string(pid) + kCmdlineFilename;
  string cmd;
  // check proc exist
  if (fs::exists(fpath)) {
    std::ifstream iss(fpath);
    for (std::string line; std::getline(iss, line);) {
      cmd = cmd + line + " ";
    }

  } else {
    // std::clog << "Command: no proc:" << pid << "\n";
    cmd = "NA";
  }

  return cmd;
}

// auto LinuxParser::UtilStr2Num(string s)
// // template <typename Type> Type LinuxParser::UtilStr2Num(string s) { return
// a + b; }
// {
//   std::string output =
//         std::regex_replace(s, std::regex("([0-9]+).*"), std::string("$1"));

// }
// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::Ram(int pid) {
  std::string fpath;
  fpath = kProcDirectory + std::to_string(pid) + kStatusFilename;
  string vmSize = "";
  long vmS_i{0};

  if (fs::exists(fpath)) {
    vmSize = UtilGetVal2(fpath, ':', "VmRSS"); //VmSize
    vmS_i = long((stof(vmSize) * 0.001));  // round

  } else {
    // std::clog << "Ram: no proc:" << pid << "\n";
  }

  // std::vector<string> vstr = UtilParseStr2Vec(uid_str, '\t');

  // return string(vstr[0]);

  return vmS_i;
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
int LinuxParser::getUid(int pid) {
  std::string fpath;
  fpath = kProcDirectory + std::to_string(pid) + kStatusFilename;
  int uid{0};
  if (fs::exists(fpath)) {
    std::string uid_str = UtilGetVal1(fpath, ':', "Uid");
    std::vector<string> vstr = UtilParseStr2Vec(uid_str, '\t');
    uid = stoi(vstr[0]);
  } else {
    // std::clog << "Uid: no proc:" << pid << "\n";
  }
  return uid;
}

string LinuxParser::Uid(int pid, mpIntInt_t const& mpPidUid) {
  // std::string fpath;
  // fpath = kProcDirectory + std::to_string(pid) + kStatusFilename;
  string uid_s = "";
  if (mpPidUid.count(pid) > 0) {
    int uid = mpPidUid.at(pid);
    uid_s = to_string(uid);
    // std::string uid_str = UtilGetVal1(fpath, ':', "Uid");
    // std::vector<string> vstr = UtilParseStr2Vec(uid_str, '\t');
    // uid = vstr[0];
  } else {
    std::clog << "Uid: no proc:" << pid << "\n";
    uid_s = "NA";
  }
  return uid_s;
}

mpIntInt_t LinuxParser::GetMapPidUid(vInt_t pids) {
  mpIntInt_t m;
  string uid_s;
  int uid_int;
  for (int pid : pids) {
    uid_int = getUid(pid);
    m[pid] = uid_int;
  }
  return m;
}

mpIntStr_t LinuxParser::GetMapUidUsrName() {
  std::string fpath;
  fpath = kPasswordPath;

  mpStrStr_t m0 = UtilMap1(fpath, ':');
  mpIntStr_t m1;
  vector<string> tmpVec;
  string uid_s;
  int uid_int;
  // for (const auto & [ key, value ] : m0) 
   for (auto i=m0.begin(); i != m0.end(); i++)
   {
     std::string key = i->first;
     std::string value = i->second;
  
     tmpVec = UtilParseStr2Vec(value, ':');
    uid_s = tmpVec[1];
    uid_int = std::stoi(uid_s);
    m1[uid_int] = key;
  }
  return m1;
}

mpIntStr_t LinuxParser::GetMapPidUsrName(vInt_t pids, mpIntInt_t mPidUid,
                                         mpIntStr_t mUidUsr) {
  mpIntStr_t m;
  int uid_int;
  string usr;
  for (int pid : pids) {
    uid_int = mPidUid[pid];
    usr = mUidUsr[uid_int];
    m[pid] = usr;
  }
  return m;
}
// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid, mpIntStr_t const& mpPidUsr) {
  std::string usr;
  if (mpPidUsr.count(pid) > 0) {
    usr = mpPidUsr.at(pid);
  } else {
    // throw std::invalid_argument("can't find key, default value would be
    // used");
    usr = "USR_NOT_FOUND";
  }
  return usr;
}
/* std::string fpath;
fpath = kPasswordPath;

std::map<std::string, std::string> m0 = UtilMap1(fpath, ':');
std::map<int, std::string> m1;
vector<string> tmpVec;
string uid_s;
int uid_int;
for (const auto& [key, value] : m0) {
  tmpVec = UtilParseStr2Vec(value, ':');
  uid_s = tmpVec[1];
  uid_int = std::stoi(uid_s);
  m1[uid_int] = key;

  // cout << key << ":" << value << "\n";
}
// preproc map to invert it and map uid -> UserName

// try to get user name
uid_s = Uid(pid);
uid_int = std::stoi(uid_s);
std::string usr{"NA"};

// std::string uid_str = UtilGetVal1(fpath, ':', "Uid");
// std::vector<string> vstr = UtilParseStr2Vec(uid_str, '\t');

// return string(vstr[0]);
*/

/* void LinuxParser::GetLinuxParseMaps(vInt_t pids)
// fill maps
{
  // mapUidUsrName = {};
  //  mapUidUsrName = LinuxParser::GetMapUidUsrName();
  // LinuxParser::mapPidUid = LinuxParser::GetMapPidUid(pids);
  // LinuxParser::mapPidUser =
  // LinuxParser::GetMapPidUsrName(pids, mapPidUid, mapUidUsrName);  // maps
  // pid->UserName
} */

// TODO: finish it with jiffies, see comment from
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float LinuxParser::ProcCpuUtil(int pid, long systemUptime) {
  std::string fpath;
  fpath = kProcDirectory + std::to_string(pid) + kStatFilename;
  // check proc exist
  float cpu_usage = 0;
  if (fs::exists(fpath)) {
    std::string s = UtilGetVal2(fpath, ' ', to_string(pid));
    vector<string> v = UtilParseStr2Vec(s, ' ');
    long utime = stoi(v[12]);
    long stime = stoi(v[13]);
    long cutime = stoi(v[14]);
    long cstime = stoi(v[15]);
    long starttime = stoi(v[20]);
    long Hertz = sysconf(_SC_CLK_TCK);
    long total_time = utime + stime;
    total_time = total_time + cutime + cstime;
    float seconds = float(systemUptime) - float(starttime / Hertz);
    cpu_usage = (float(total_time / Hertz) / seconds);
  } else {
    // std::clog << "UpTime: no proc:" << pid << "\n";
  }

  return cpu_usage;
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid, long systemUptime) {
  std::string fpath;
  fpath = kProcDirectory + std::to_string(pid) + kStatFilename;
  long uptime{0};
  // check proc exist
  if (fs::exists(fpath)) {
    std::string s = UtilGetVal2(fpath, ' ', to_string(pid));
    vector<string> v = UtilParseStr2Vec(s, ' ');
    uptime = stoi(v[20]);
    uptime =systemUptime -  uptime / sysconf(_SC_CLK_TCK);
  } else {
    // std::clog << "UpTime: no proc:" << pid << "\n";
  }

  return uptime;
}

vector<string> LinuxParser::UtilParseStr2Vec(string s, char delim) {
  vector<string> vstrings;
  std::stringstream ss(s);
  std::string tmp;
  while (getline(ss, tmp, delim)) {
    vstrings.push_back(tmp);
  }
  return vstrings;
}