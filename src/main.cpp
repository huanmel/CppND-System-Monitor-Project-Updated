#include <iostream>
#include <map>

#include "linux_parser.h"
#include "ncurses_display.h"
#include "system.h"
int main() {
  System system;
  NCursesDisplay::Display(system);
  // std::map<std::string, std::string> mp1 = LinuxParser::UtilMap1(
  //     LinuxParser::kProcDirectory + LinuxParser::kStatFilename, ' ');
  // std::cout << "END\n";

  // std::vector<Process> proc=system.Processes();
  /* std::vector<int> pid_v = LinuxParser::Pids();
  // std::string uid = LinuxParser::Uid(v);
  // std::string s= LinuxParser::User(pid_v[10]);
  std::string usr;
  std::string mem;
  long uptime;
  std::string cmd;
  for (int v : pid_v) {
    usr = LinuxParser::User(v);
    mem = LinuxParser::Ram(v);
    uptime = LinuxParser::UpTime(v);
    cmd = LinuxParser::Command(v);
    std::cout << v << " : user:" << usr << " mem:" << mem << " time:" << uptime
              << "\n";
    std::cout << "cmd: " << cmd << "\n";
  } */
}