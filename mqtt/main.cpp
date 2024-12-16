#include "mqtt.hpp"
#include "common.hpp"
#include "task_manager.hpp"
#include "actuator.hpp"
#include "debug_output.hpp"
#include "automation/registry.hpp"
#include <map>
#include <chrono>
#include <thread>
#include <iomanip>
#include <spdlog/spdlog.h>
//#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


class IntTime {
public:
    IntTime(int hour_, int minute_) :
        hour(hour_),
        minute(minute_)
    {}
    int getHour() const {
        return hour;
    }
    int getMinute() const {
        return minute;
    }
    int getTime() const {
        return hm2time(hour, minute);
    }

    void operator+(int minutes) {
        minute += minutes;
        if (minute >= 60) {
            hour += minute / 60;
            minute %= 60;
            if (hour >= 24) {
                hour %= 24;
            }
        }
    }
private:
    int hour;
    int minute;
};


/**
 * @brief The main function of the program.
 */
int main() {
    // initialize logging
    //
    //auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/switch_device.log", true);
    auto syslog_sink = std::make_shared<spdlog::sinks::syslog_sink_mt>("switch_device", LOG_PID, LOG_USER, true);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    spdlog::sinks_init_list sink_list = {syslog_sink, console_sink};
    auto logger = std::make_shared<spdlog::logger>("yaha", sink_list.begin(), sink_list.end());
    logger->set_pattern("%H:%M:%S %L %^%v%$");
    spdlog::set_default_logger(logger);

    // spdlog::info("this is info message");
    // spdlog::warn("this is warning message");
    // spdlog::error("this is error message");

    // initialize system
    //
    auto mqtt = std::make_unique<Mqtt>();
    auto actuator = std::make_shared<Actuator>(dynamic_cast<IOutput*>(mqtt.get()));
    TaskManager taskManager;
    automation::Registry automations(actuator);
    DebugOutput debugOutput;
    DualEventManager evManager(&taskManager, mqtt->getEventManager());

    automations.load("automation.json", evManager);

    debugOutput.registerEvents(evManager);

    evManager.subscribe(EventId::TIME, *actuator);

    while (true) {
        mqtt->execute();
        taskManager.execute();

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    return 0;
}
