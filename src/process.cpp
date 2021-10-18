#include <unistd.h>
#include <cctype>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include "format.h"

using namespace std;
void Salida(string str, string value) 
{
    cout << str << "." << value << "." << endl;
}

Process::Process(int pid)
{
    pid_ = pid;
    command_ = LinuxParser::Command(pid);
    user_ = LinuxParser::User(pid);
    ram_ = LinuxParser::Ram(pid);
    total_time_ = LinuxParser::ActiveJiffies(pid);
    start_time_ = LinuxParser::UpTime(pid);
    long sys_uptime=LinuxParser::UpTime();              
    seconds_ = (float)sys_uptime - ((float)start_time_);
}

// TODO: Return this process's ID
int Process::Pid() const { 
    return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    float cpu_usage;
    long sys_uptime=LinuxParser::UpTime();              
    // long Hertz = sysconf(_SC_CLK_TCK);
    // float seconds = (float)sys_uptime - ((float)start_time_ / (float)Hertz);
    
    float seconds = (float)sys_uptime - ((float)start_time_);
    // float cpu_usage = 100 * (total_time_  / seconds);
    
    if(seconds != 0.0)
        cpu_usage = total_time_  / seconds;
    else
        cpu_usage = 0;

    return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() const{ 
    return command_; 
}

// TODO: Return this process's memory utilization
string Process::Ram(){ 
    long ram=stol(ram_) / 1024;
    return to_string(ram);
}

// TODO: Return the user (name) that generated this process
string Process::User() const{ 
    return user_; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return seconds_;
 }
