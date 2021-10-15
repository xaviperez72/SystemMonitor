#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
// VERIFIED OK
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
// VERIFIED OK
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
// VERIFIED OK
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
// % of Utilization
// VERIFIED OK
float LinuxParser::MemoryUtilization() { 
  float uti, mem_free_perc; 
  long mem[2];
  int count_lines = 0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && count_lines < 2) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      mem[count_lines] = std::stol(value);
      count_lines++;
    }
  }
   mem_free_perc = (float) (mem[1] * 100) / mem[0];
  uti = 100.0 - mem_free_perc;
  return uti; 
}


// TODO: Read and return the system uptime
// VERIFIED OK
long LinuxParser::UpTime() { 
  long uptime = 0;
  string line, uptime_seconds;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_seconds;
    uptime = std::stol(uptime_seconds);
    return uptime;
  }
  return 0;
  }

// TODO: Read and return the number of jiffies for the system
// Nobody call it??? 
long LinuxParser::Jiffies() { 
  long ret;
  ret = LinuxParser::UpTime() * sysconf(_SC_CLK_TCK);
  std::cout << "LinuxParser::Jiffies:" << ret << ".\n";
  exit(1);
  return ret;

// This is the number of ticks the system made since the last boot. To calculate it, you need two ingradients: 
// System uptime (you should implment it in LinuxParser::UpTime) and system frequency which is the number of ticks per second 
// (you can get it using the following function ( sysconf(_SC_CLK_TCK) ). By multiplying them you will get the total jiffies. 

// Should return sum of `ActiveJiffies()` and `IdleJiffies()` (you may not use this one at all, as the prev two are sufficient)
}

// TODO: Read and return the number of active jiffies for the system
// In the ( /proc/stat), you need to extract the first line, i.e. the line starts with (cpu). 
// You will get ten tokens. Summing them and you will get the ActiveJiffies.
// Nobody call it??? 
long LinuxParser::ActiveJiffies() { 
  long value = 0;
  vector<string> cu = LinuxParser::CpuUtilization();
  for(auto str : cu) 
  {
     value += std::stol(str);
  }
  return 0; 
  // ProcDirectory+kStatFilename line by line and in there the 4th and 5th value in a line are idle jiffies and the rest can be counted as active jiffies.
}


// TODO: Read and return the number of idle jiffies for the system
// user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
// idleJiffies = idle + iowait.
// Nobody call it ?????
long LinuxParser::IdleJiffies() { 
  long value = 0;
  vector<string> cu = LinuxParser::CpuUtilization();
  if(cu.size()>= 4)
     value=std::stol(cu[kIdle_]) * std::stol(cu[kIOwait_]);
  return value;
}

// TODO: Read and return CPU utilization
// VERIFIED OK
vector<string> LinuxParser::CpuUtilization() {            // to confirm ??
  vector<string> cu;
  string line, str;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> str;  // get "cpu"
    while (linestream >> str) {
      cu.push_back(str);
    }
  }
  return cu;
}

// TODO: Read and return the total number of processes
// VERIFIED OK
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  long value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if( key == "processes") {
          return value;
      }
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
// VERIFIED OK
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  long value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if( key == "procs_running") {
          return value;
      }
    }
  }
  return 0; 
}

// TODO: Read and return the number of active jiffies for a PID
// VERIFIED OK
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string key;
  long value = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    // get the 14th 15th 16th 17th values
    for (int i = 1; i <= 17; i++) {
      linestream >> key;
      switch(i)
      { case 14: case 15: case 16: case 17:
            value += stol(key);
            break;
        default:
            break;
      }
    }
  }
  return value;
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
// VERIFIED OK
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), '\0', ' ');
  }
  return line; 
}

// TODO: Read and return the memory used by a process
// VERIFIED OK
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if( key == "VmSize:") {
          return value;
      }
    }
  }
  return value;
}

// TODO: Read and return the user ID associated with a process
// VERIFIED OK 
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if( key == "Uid:") {
          return value;
      }
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
// VERIFIED OK 
string LinuxParser::User(int pid) { 
  string line, user, pass, uid, gid, file_uid=LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> pass >> uid >> gid;
      if( uid == file_uid) {
          return user;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// VERIFIED OK
long LinuxParser::UpTime(int pid) { 
  string line;
  string key;
  long value = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    // get the 22nd 
    for (int i = 1; i <= 22; i++) {
      linestream >> key;
      if(i==22)      {
          value += stol(key);
      }
    }
  }
  return value;
}
