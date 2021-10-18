#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include "processor.h"
#include "linux_parser.h"

using namespace LinuxParser;
using namespace std;

long convert_stol(string s) { return std::stol(s);}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // static int counter=0;
    std::vector<std::string> b = LinuxParser::CpuUtilization();    
    std::vector<long>  curr;
    curr.resize(b.size());
    std::transform(b.begin(), b.end(), curr.begin(), convert_stol);     // Move all string b to curr.

    if(prev.size()==0) {
        prev.resize(b.size());      // Avoid coredump at the first time
    }

// Extracted from:
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

// CPU
//  0       1       2       3       4        5        6       7       8       9
// kUser_   kNice_ kSystem_ kIdle_ kIOwait_  kIRQ_ kSoftIRQ_ kSteal_ kGuest_ kGuestNice

    //  PrevIdle = previdle     + previowait
        PrevIdle = prev[kIdle_] + prev[kIOwait_];
    //  Idle = idle         + iowait
        Idle = curr[kIdle_] + curr[kIOwait_];

    //  PrevNonIdle = prevuser       + prevnice      + prevsystem     + previrq     + prevsoftirq    + prevsteal
        PrevNonIdle = prev[kUser_] + prev[kNice_] + prev[kSystem_] + prev[kIRQ_] + prev[kSoftIRQ_] + prev[kSteal_];
    //  NonIdle = user          + nice          + system         + irq         + softirq        + steal
        NonIdle = curr[kUser_] + curr[kNice_] + curr[kSystem_] + curr[kIRQ_] + curr[kSoftIRQ_] + curr[kSteal_];

        PrevTotal = PrevIdle + PrevNonIdle;
        Total = Idle + NonIdle;

    //  differentiate: actual value minus the previous one
        totald = Total - PrevTotal;
        idled = Idle - PrevIdle;

        CPU_Percentage = (totald - idled)/totald;
/*
    cout << "PrevIdle   :|" << PrevIdle  << "|\n";
    cout << "Idle       :|" << Idle  << "|\n";
    cout << "PrevNonIdle:|" << PrevNonIdle  << "|\n";
    cout << "NonIdle    :|" << NonIdle  << "|\n";
    cout << "PrevTotal  :|" << PrevTotal  << "|\n";
    cout << "Total      :|" << Total  << "|\n";
    cout << "totald     :|" << totald  << "|\n";
    cout << "idled      :|" << idled  << "|\n";

    if(++counter > 2)
        exit(1);
    else
        sleep(5);

    cout << "CPU_Percentage:|" << CPU_Percentage  << "|\n";*/

    prev = curr;

    return CPU_Percentage; 
}