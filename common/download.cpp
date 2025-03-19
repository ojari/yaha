#include <string.h>
#include <curl/curl.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "download.hpp"

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
