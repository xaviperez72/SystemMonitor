#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float PrevIdle{0}, Idle{0}, PrevNonIdle{0}, NonIdle{0}, PrevTotal{0}, Total{0}, totald{0}, idled{0}, CPU_Percentage{0};
  std::vector<long>  prev;
};

#endif