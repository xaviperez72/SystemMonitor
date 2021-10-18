#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid() const;                               // TODO: See src/process.cpp
  std::string User() const;                      // TODO: See src/process.cpp
  std::string Command() const;                   // TODO: See src/process.cpp
  float CpuUtilization() const;            // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const 
  { 
    return a.CpuUtilization() < this->CpuUtilization();
  }

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string command_;
  std::string ram_;

  float total_time_;
  long start_time_;
  long seconds_;
};

#endif