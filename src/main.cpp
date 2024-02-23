#include <map>

#include "linux_parser.h"
#include "ncurses_display.h"
#include "system.h"
#include <iostream>
int main() {
  System system;
  NCursesDisplay::Display(system);
  // std::map<std::string, std::string> mp1 = LinuxParser::UtilMap1(
  //     LinuxParser::kProcDirectory + LinuxParser::kStatFilename, ' ');
  // std::cout << "END\n";
}