#pragma once
#include <fstream>
#include "../event_data.hpp"
#include "../task.hpp"
#include "spdlog/spdlog.h"

namespace task {

class MemoryUsageReader : public ITask {
public:
    explicit MemoryUsageReader(const std::string& filePath_ = "/proc/meminfo")
        : filePath(filePath_) {}

    void execute() override{
        if (errorOccurred) {
            return;  // Skip reading if an error has occurred previously
        }
        std::ifstream meminfoFile(filePath);
        if (!meminfoFile.is_open()) {
            spdlog::error("Error opening {}", filePath);
            errorOccurred = true;  // Set error flag to prevent further attempts
            return;
        }

        std::string line;
        while (std::getline(meminfoFile, line)) {
            if (line.find("MemTotal:") == 0) {
                sscanf(line.c_str(), "MemTotal: %lu kB", &totalMem);
            } else if (line.find("MemFree:") == 0) {
                sscanf(line.c_str(), "MemFree: %lu kB", &freeMem);
            } else if (line.find("Buffers:") == 0) {
                sscanf(line.c_str(), "Buffers: %lu kB", &buffers);
            } else if (line.find("Cached:") == 0) {
                sscanf(line.c_str(), "Cached: %lu kB", &cached);
            }
        }
        meminfoFile.close();

        usedMem = totalMem - freeMem - buffers - cached;
    }

    unsigned long getTotalMem() const { return totalMem; }
    unsigned long getFreeMem() const { return freeMem; }
    unsigned long getBuffers() const { return buffers; }
    unsigned long getCached() const { return cached; }
    unsigned long getUsedMem() const { return usedMem; }

private:
    unsigned long totalMem;
    unsigned long freeMem;
    unsigned long buffers;
    unsigned long cached;
    unsigned long usedMem;

private:
    std::string filePath;
    bool errorOccurred = false;
};

}  // namespace task
