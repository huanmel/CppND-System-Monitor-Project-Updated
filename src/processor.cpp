#include "processor.h"

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  // static long previowait, previdle, prevuser, prevnice, prevsystem, previrq,
  // prevsoftirq, prevsteal;
  long iowait, idle, user, nice, system, irq, softirq, steal;
  // float PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald,
  // idled;
  float Idle, NonIdle, Total, totald, idled;

  float procUtil = 0;
  std::vector<std::string> cpuUtil = LinuxParser::CpuUtilization();
  user = std::stol(cpuUtil[LinuxParser::CPUStates::kUser_]);
  nice = std::stol(cpuUtil[LinuxParser::CPUStates::kNice_]);
  system = std::stol(cpuUtil[LinuxParser::CPUStates::kSystem_]);
  idle = std::stol(cpuUtil[LinuxParser::CPUStates::kIdle_]);
  iowait = std::stol(cpuUtil[LinuxParser::CPUStates::kIOwait_]);
  irq = std::stol(cpuUtil[LinuxParser::CPUStates::kIRQ_]);
  softirq = std::stol(cpuUtil[LinuxParser::CPUStates::kSoftIRQ_]);
  steal = std::stol(cpuUtil[LinuxParser::CPUStates::kSteal_]);

  Idle = idle + iowait;

  NonIdle = user + nice + system + irq + softirq + steal;

  Total = Idle + NonIdle;

  // #differentiate : actual value minus the previous one
  totald = Total - _prevTotal;
  idled = Idle - _prevIdle;

  procUtil = float(totald - idled) / float(totald);

  _prevIdle = idle;
  _prevTotal = Total;

  _utilization = procUtil;

  return _utilization;
}