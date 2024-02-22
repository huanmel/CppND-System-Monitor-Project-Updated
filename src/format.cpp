#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{
  int  hours, minutes;
  minutes = seconds / 60;
  hours = minutes / 60;
  string s;
  s = std::to_string(int(hours)) + ":" + std::to_string(int(minutes % 60)) +
      ":" + std::to_string(int(seconds% 60));
  return s; 
}