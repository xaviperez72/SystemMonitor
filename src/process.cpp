#include <unistd.h>
#include <cctype>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

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
    /* 
    Salida("PID", to_string(pid));
    Salida("COMMAND", command_);
    Salida("USER", user_);
    Salida("RAM", ram_);
    Salida("TOTAL_TIME", to_string(total_time_));
    Salida("START_TIME", to_string(start_time_));
*/
    // sleep(5);
}

// TODO: Return this process's ID
int Process::Pid() { 
    return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    long sys_uptime=LinuxParser::UpTime();              
    long Hertz = sysconf(_SC_CLK_TCK);
    float seconds = (float)sys_uptime - ((float)start_time_ / (float)Hertz);
    float cpu_usage = 100 * ((total_time_ / (float)Hertz) / seconds);
    return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return command_; 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return ram_;
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return user_; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return start_time_;
 }
