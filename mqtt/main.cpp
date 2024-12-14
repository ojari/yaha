#include "mqtt.hpp"
#include "common.hpp"
#include "task_manager.hpp"
#include "actuator.hpp"
#include "automation/registry.hpp"
#include <map>
#include <chrono>
#include <thread>
#include <iomanip>
#include <spdlog/spdlog.h>
//#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


struct DebugOutput : public IObserver {
    void onChange(const IEventData& value) override {
        if (value.id() == EventId::TIME) {
            time = value.getInt();
        }
        else if (value.id() == EventId::TEMPERATURE) {
            // do not show temperature changes
        }
        else {
            if (value.isInt()) {
                spdlog::info("{}: {} changed to {}", time2str(time), value.name(), value.getInt());
            } else {
                spdlog::info("{}: {} changed to {}", time2str(time), value.name(), value.getFloat());
            }
        }
    }

private:
    int time {-1};
};


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

struct DummyHistory : public DataInsertHistory {
    void insert(const DataHistory& data) override {
        // dummy implementation: do nothing
    }
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
    DummyHistory history;
    auto mqtt = std::make_unique<Mqtt>();
    auto actuator = std::make_shared<Actuator>(dynamic_cast<IOutput*>(mqtt.get()));
    TaskManager taskManager;
    automation::Registry automations(actuator);
    DebugOutput debugOutput;
    DualEventManager evManager(&taskManager, mqtt->getEventManager());

    automations.load("automation.json", evManager);

    evManager.subscribe(EventId::TEMPERATURE, debugOutput);
    evManager.subscribe(EventId::TEMPERATURE_ROOM, debugOutput);
    evManager.subscribe(EventId::TIME, debugOutput);
    evManager.subscribe(EventId::BUTTON_LIBRARY, debugOutput);
    evManager.subscribe(EventId::BUTTON_LIVING_ROOM, debugOutput);
    evManager.subscribe(EventId::TIME, *actuator);

    std::vector<int> timeRange1 = {1000, 1300};
    std::vector<int> timeRange2 = {1200, 1500};
    //automations.add(evManager, "Demo1", automation::LIGHTS, timeRange1);
    //automations.add(evManager, "Demo2", automation::LIGHTS, timeRange2);

    automations.add(evManager, "ButtonKirjasto2", automation::SWITCH, timeRange1);

    //automations.add(evManager, actuator,  ConfigController { "Demo1", "Lights", "", 1000, 1300 });
    //automations.add(evManager, actuator,  ConfigController { "Demo2", "Lights", "", 1200, 1500 });
    while (true) {
        mqtt->execute();
        taskManager.execute();

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    return 0;
}
