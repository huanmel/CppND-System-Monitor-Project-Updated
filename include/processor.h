#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // DONE: See src/processor.cpp

  // DONE with static inside function: Declare any necessary private members
 private:
  float _utilization;
  long _prevTotal{0};
  long _prevIdle{0};
};

#endif