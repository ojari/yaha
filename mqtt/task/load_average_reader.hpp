#pragma once
#include <fstream>
#include "../task.hpp"
#include "spdlog/spdlog.h"

namespace task {

class LoadAverageReader : public ITask {
public:
    explicit LoadAverageReader(const std::string& loadavgFilePath = "/proc/loadavg")
        : filepath(loadavgFilePath)
        {}

    void execute() override {
        if (errorOccurred) {
            return;  // Skip execution if an error has occurred previously
        }

        std::ifstream loadavgFile(filepath);
        if (!loadavgFile.is_open()) {
            spdlog::error("Error opening {}", filepath);
            errorOccurred = true;  // Set error flag to prevent further attempts
            return;
        }
        float load1 = 0.0f;
        float load5 = 0.0f;
        float load15 = 0.0f;

        loadavgFile >> load1 >> load5 >> load15;
        loadavgFile.close();

        // event.set(load5);
        last_value = load5;
    }

    float getLoad() const {
        return last_value;
    }

private:
    std::string filepath;
    bool errorOccurred = false;
    float last_value;
};

}  // namespace task
