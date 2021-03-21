#include <string>
#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string res, tmp;
    unsigned int hour, minute, second;
    hour = seconds / (60 * 60);
    tmp = std::to_string(hour);
    if(tmp.size() < 2) tmp = "0" + tmp;
    res += tmp + ":";
    seconds %= (60 * 60);
    minute = seconds / 60;
    tmp = std::to_string(minute);
    if(tmp.size() < 2) tmp = "0" + tmp;
    res += tmp + ":";
    second = seconds % 60;
    tmp = std::to_string(second);
    if(tmp.size() < 2) tmp = "0" + tmp;
    res += tmp;
    return res;
}