#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <string>
#include <chrono>

using namespace std;

long get_epoch()
{
    auto now = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();
    cout << "Current epoch time: " << now << endl;
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

    cout << utc->tm_mday << "/" << (utc->tm_mon + 1) << " "
         << setfill('0') << setw(2) << utc->tm_hour
         << ":"
         << setfill('0') << setw(2) << utc->tm_min;
    
    return ((utc->tm_hour == 23) && (utc->tm_min == 0));
}

long convert2epoch(const char *time_str)
{
    istringstream ss(time_str);
    std::tm tmStruct = {};
    ss >> get_time(&tmStruct, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        cerr << "Error parsing date-time string." << std::endl;
        return 0;
    }

    auto tp = chrono::system_clock::from_time_t(mktime(&tmStruct));
    auto epoch = chrono::duration_cast<chrono::seconds>(tp.time_since_epoch()).count();

    return (long)epoch;
}
