#include <string.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>
#include "utils.hpp"

long get_epoch()
{
    auto now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Current epoch time: " << now << std::endl;
    return static_cast<long>(now);
}

/** print epoch into console format "<day>/<month> <hour>:<minute>"
 *
 *  returns is time is 23:00 (last one in weather or spot data)
 */
bool print_epoch(long epoch)
{
    const time_t epochtt = (time_t)epoch;
    tm* utc = localtime(&epochtt);

    std::cout << utc->tm_mday << "/" << (utc->tm_mon + 1) << " "
              << std::setfill('0') << std::setw(2) << utc->tm_hour
              << ":"
              << std::setfill('0') << std::setw(2) << utc->tm_min;

    return ((utc->tm_hour == 23) && (utc->tm_min == 0));
}

long convert2epoch(const char *time_str)
{
    std::istringstream ss(time_str);
    std::tm tmStruct = {};
    ss >> std::get_time(&tmStruct, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        std::cerr << "Error parsing date-time string." << std::endl;
        return 0;
    }

    auto tp = std::chrono::system_clock::from_time_t(mktime(&tmStruct));
    auto epoch = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();

    return (long)epoch;
}
