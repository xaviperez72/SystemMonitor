#include <string>
#include <iostream>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

#define SEC_IN_MIN (60)
#define MIN_IN_HR (60)
#define SEC_IN_HR (SEC_IN_MIN * MIN_IN_HR)

string Format::ElapsedTime(long seconds) { 
    long hours, mins, secs;

    hours = seconds/SEC_IN_HR;
    seconds = seconds%SEC_IN_HR;
    mins = seconds/MIN_IN_HR;
    secs = seconds%SEC_IN_MIN;
    std::ostringstream stream;
    stream << std::setw(2) << std::setfill('0') << hours << ":" 
     << std::setw(2) << std::setfill('0') << mins << ":"
     << std::setw(2) << std::setfill('0') << secs;

    return stream.str();
}