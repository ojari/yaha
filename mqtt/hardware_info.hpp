#pragma once

#include <spdlog/spdlog.h>
#include <unistd.h>
#include <fstream>
#include "common.hpp"
#include "observable.hpp"
#include "event_data.hpp"

class LoadAverageReader {
public:
    explicit LoadAverageReader(const std::string& loadavgFilePath = "/proc/loadavg")
        : loadavgFilePath_(loadavgFilePath),
        load(EventId::LOAD_AVG, 0.0f) {}

    bool Read() {
        std::ifstream loadavgFile(loadavgFilePath_);
        if (!loadavgFile.is_open()) {
            spdlog::error("Error opening {}", loadavgFilePath_);
            return false;
        }
        float load1 = 0.0f;
        float load5 = 0.0f;
        float load15 = 0.0f;

        loadavgFile >> load1 >> load5 >> load15;
        loadavgFile.close();

        load.set(load5);
        return true;
    }

    float getLoad() const {
        return load.getFloat();
    }

private:
    EventData load;
    std::string loadavgFilePath_;
};

//-----------------------------------------------------------------------------
class MemoryUsageReader {
public:
    explicit MemoryUsageReader(const std::string& meminfoFilePath = "/proc/meminfo")
        : meminfoFilePath(meminfoFilePath) {}

    bool Read() {
        std::ifstream meminfoFile(meminfoFilePath);
        if (!meminfoFile.is_open()) {
            spdlog::error("Error opening {}", meminfoFilePath);
            return false;
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

        return true;
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
    std::string meminfoFilePath;
};

//-----------------------------------------------------------------------------
#ifndef WIN32
#include <unistd.h>
#endif

class ProcessMemoryReader : public Observable {
public:
    ProcessMemoryReader()
        : statusFilePath_("/proc/" + std::to_string(getpid()) + "/status") {}

    bool Read() {
        std::ifstream statusFile(statusFilePath_);
        if (!statusFile.is_open()) {
            spdlog::error("Error opening {}", statusFilePath_);
            return false;
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
        return true;
    }

    unsigned int getVmRSS() const {
        return vmRss.getInt(-1);
    }

private:
    void update(float value) {
        if (vmRss.getInt() != value) {
            vmRss.set(value);
            notify(vmRss);
        }
    }

    EventData vmRss {EventId::PROC_MEM, 0};
    std::string statusFilePath_;
};
