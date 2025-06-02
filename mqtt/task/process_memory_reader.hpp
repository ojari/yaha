#pragma once

#include <spdlog/spdlog.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <fstream>
#include "../common.hpp"
#include "../observable.hpp"
#include "../event_data.hpp"

#ifndef WIN32
#include <unistd.h>
#endif

class ProcessMemoryReader : public Observable, public ITask {
public:
    ProcessMemoryReader()
        : filePath("/proc/" + std::to_string(getpid()) + "/status") {}

    void execute() override {
        if (errorOccurred) {
            return;  // Skip reading if an error has occurred previously
        }
        std::ifstream statusFile(filePath);
        if (!statusFile.is_open()) {
            spdlog::error("Error opening {}", filePath);
            errorOccurred = true;  // Set error flag to prevent further attempts
            update(-1);  // Notify with an error value
            return;
        }

        std::string line;
        while (std::getline(statusFile, line)) {
            if (line.find("VmRSS:") == 0) {
                long value = 0;
                sscanf(line.c_str(), "VmRSS: %lu kB", &value);
                update(value);
            }
        }
        statusFile.close();
    }

    unsigned int getVmRSS() const {
        return vmRss.getInt(-1);
    }

private:
    void update(float value) {
        if (vmRss.set(value)) {
            notify(vmRss);
        }
    }

    EventData vmRss {EventId::PROC_MEM, 0};
    std::string filePath;
    bool errorOccurred = false;
};
