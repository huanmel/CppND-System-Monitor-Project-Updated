#include "processor.h"

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  static int previowait, previdle, prevuser, prevnice, prevsystem, previrq,
      prevsoftirq, prevsteal;
  int iowait, idle, user, nice, system, irq, softirq, steal;
  float PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald, idled;
  static bool IsInit = false;
  float procUtil = 0;
  std::vector<std::string> cpuUtil = LinuxParser::CpuUtilization();
  user = std::stoi(cpuUtil[0]);
  nice = std::stoi(cpuUtil[1]);
  system = std::stoi(cpuUtil[2]);
  idle = std::stoi(cpuUtil[3]);
  iowait = std::stoi(cpuUtil[4]);
  irq = std::stoi(cpuUtil[5]);
  softirq = std::stoi(cpuUtil[6]);
  steal = std::stoi(cpuUtil[7]);
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