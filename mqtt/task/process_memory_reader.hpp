#pragma once
#include <spdlog/spdlog.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <fstream>
#include "../common.hpp"

namespace task {

class ProcessMemoryReader : public ITask {
public:
    ProcessMemoryReader(EventBus& evbus) :
        evbus(evbus),
        filePath("/proc/" + std::to_string(getpid()) + "/status")
    {}

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

private:
    void update(float value) {
        if (last_value != value) {
            evbus.publish<PcEvent>(PcEvent(PcEvent::process_memory, static_cast<unsigned long>(value)));
            last_value = value;
        }
    }

    EventBus& evbus;
    std::string filePath;
    bool errorOccurred = false;
    float last_value = 0.0f;
};

} // namespace task
