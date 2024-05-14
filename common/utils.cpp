#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <string>
#include <chrono>
#include <curl/curl.h>

using namespace std;


// Callback function to write received data into a string
size_t curl_write_cb(void *contents, size_t size, size_t nmemb, std::string* output)
{
    size_t real_size = size * nmemb;

    // cout << "callback size:" << size << " nmemb:" << nmemb << endl;
    output->append(static_cast<char*>(contents), real_size);

    return real_size;
}

void download(const char *url, const char *cache_file)
{
    CURL *curl;
    CURLcode res;
    string response;

    // Initialize libcurl
    curl = curl_easy_init();
    if (curl) {
        // Set the URL to fetch
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the callback function to write response data into the 'response' variable
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed:" << curl_easy_strerror(res) << endl;
        } else {
            cout << "writing to " << cache_file << endl;

            std::ofstream fout(cache_file);
            fout << response << endl;
            fout.close();
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }
}

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
