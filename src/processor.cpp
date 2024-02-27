#include "processor.h"

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  static long previowait, previdle, prevuser, prevnice, prevsystem, previrq,
      prevsoftirq, prevsteal;
  long iowait, idle, user, nice, system, irq, softirq, steal;
  float PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald, idled;
  static bool IsInit = false;
  float procUtil = 0;
  std::vector<std::string> cpuUtil = LinuxParser::CpuUtilization();
  user = std::stol(cpuUtil[0]);
  nice = std::stol(cpuUtil[1]);
  system = std::stol(cpuUtil[2]);
  idle = std::stol(cpuUtil[3]);
  iowait = std::stol(cpuUtil[4]);
  irq = std::stol(cpuUtil[5]);
  softirq = std::stol(cpuUtil[6]);
  steal = std::stol(cpuUtil[7]);
  // guest = std::stoi(cpuUtil[8]);
  // guest_nice = std::stoi(cpuUtil[9]);
  if (IsInit) {
    PrevIdle = float(previdle + previowait);
    Idle = idle + iowait;
    PrevNonIdle =
        prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    NonIdle = user + nice + system + irq + softirq + steal;

    PrevTotal = PrevIdle + PrevNonIdle;
    Total = Idle + NonIdle;

    // #differentiate : actual value minus the previous one
    totald = Total - PrevTotal;
    idled = Idle - PrevIdle;

    procUtil = float(totald - idled) / float(totald);

  } else
  // showing first time
  {
    procUtil = 0;
  }

  prevuser = user;
  prevnice = nice;
  prevsystem = system;
  previdle = idle;
  previowait = iowait;
  previrq = irq;
  prevsoftirq = softirq;
  prevsteal = steal;
  IsInit = true;

  _utilization = procUtil;

  return _utilization;
}