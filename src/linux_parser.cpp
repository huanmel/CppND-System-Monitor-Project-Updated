#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
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
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::map<std::string, std::string> m =
      UtilMap1(kProcDirectory + kMeminfoFilename, ':');
  // get values and process them
  // prepare lambda helper for it
  auto proc_memstrval = [](std::string strval) {
    std::string output =
        std::regex_replace(strval, std::regex("([0-9]+).*"), std::string("$1"));

    int val = std::stoi(output);
    return val;
  };
  int memTotal = proc_memstrval(m["MemTotal"]);
  int memFree = proc_memstrval(m["MemFree"]);
  // int memAvailable=proc_memstrval(m["MemAvailable"]);
  // int memBuffer=proc_memstrval(m["Buffers"]);
  float memUtil = float(memTotal - memFree) / float(memTotal);

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

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> vstrings;
  string tmp;
  //  std::map<std::string, std::string> m = UtilMap1(fname, delim);
  string s = UtilGetVal1(kProcDirectory + kStatFilename, ' ', "cpu");
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
    throw std::invalid_argument("can't find key, default value would be used");
    // throw waxrning("this is a warning");
  }
  return s;
};

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int procs = 0;
  // long idletime = 0;
  // string line;
  // string key;
  // string value;
  string value = UtilGetVal1(kProcDirectory + kStatFilename, ' ', "processes");
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

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
