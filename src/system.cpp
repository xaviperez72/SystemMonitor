#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <cmath>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  std::vector<int> vec_pids{LinuxParser::Pids()};

/*
  set<int> ant_pids;
  for(Process const& pr : processes_) {
      ant_pids.insert(pr.Pid());
  }
  
  // Add new pids
  for(int pid: vec_pids){
      if(ant_pids.find(pid) == ant_pids.end())
      processes_.emplace_back(Process(pid));
  }
*/
  processes_.clear();

  for(auto pid: vec_pids) 
  {
      Process process(pid);
      processes_.push_back(process);
  }
  
  std::sort(processes_.begin(),processes_.end(),std::less<Process>());

  return processes_; 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses();
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel();
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
}