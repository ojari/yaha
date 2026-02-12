// #include <unistd.h>
#include <spdlog/spdlog.h>
#include <thread>
#include <iomanip>
#include <fstream>
#include <spdlog/spdlog.h>
#include "mqtt.hpp"
#include "common.hpp"
#include "debug_output.hpp"
#include "event_bus.hpp"
#include "automation/registry.hpp"
#include "application.hpp"
// #include <spdlog/sinks/basic_file_sink.h>
#ifndef WIN32
#include <spdlog/sinks/syslog_sink.h>
#endif
#include <spdlog/sinks/stdout_color_sinks.h>


/*void writePidToFile(const std::string& filePath) {
    std::ofstream pidFile(filePath);
    if (pidFile.is_open()) {
        pidFile << getpid();
        pidFile.close();
    } else {
        spdlog::error("Unable to open file: {}", filePath);
    }
}*/

void initializeLogger() {
    // auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/switch_device.log", true);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#ifndef WIN32
    auto syslog_sink = std::make_shared<spdlog::sinks::syslog_sink_mt>("yaha", LOG_PID, LOG_USER, true);
    spdlog::sinks_init_list sink_list = { syslog_sink };
#else
    spdlog::sinks_init_list sink_list = { console_sink };
#endif

    auto logger = std::make_shared<spdlog::logger>("yaha", sink_list.begin(), sink_list.end());
    logger->set_pattern("%H:%M:%S %L %^%v%$");
    spdlog::set_default_logger(logger);
}

/**
 * @brief The main function of the program.
 */
int main(int argc, char* argv[]) {
    initializeLogger();
    
    // Command line parsing using modern C++
    // Usage: program [--create] [--simulate] [devicesFile] [automationFile]
    std::string devicesFile = "devices.json";
    std::string automationFile = "automation.json";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (devicesFile == "devices.json") {  // except that deviceFile content is changed
            devicesFile = arg;
        } else if (automationFile == "automation.json") {
            automationFile = arg;
        }
    }

    // initialize system
    //
    EventBus evBus;
    DeviceMessageRouter devRouter(devicesFile, evBus);
    BridgeMessageRouter bridgeRouter;
    auto mqtt = std::make_shared<Mqtt>(devRouter, bridgeRouter);
    automation::Registry automations(mqtt, evBus);
    task::SimpleCalcSun simpleCalcSun(evBus);
    DebugOutput debugOutput;

    automations.load(automationFile, evBus);

    debugOutput.registerEvents(evBus);

    Application app;
    TimerSlow timer1(evBus, app);
    TimerFast timer2(evBus, app);

    app.run();

    return 0;
}
