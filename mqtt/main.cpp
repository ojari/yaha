// #include <unistd.h>
#include <spdlog/spdlog.h>
#include <thread>
#include <iomanip>
#include <fstream>
#include <spdlog/spdlog.h>
#include "mqtt.hpp"
#include "common.hpp"
#include "data/sourcesqlite.hpp"
#include "data/tables.hpp"
#include "debug_output.hpp"
#include "event_bus.hpp"
#include "history.hpp"
#include "automation/registry.hpp"
#include "application.hpp"
#include "int_time.hpp"
// #include <spdlog/sinks/basic_file_sink.h>
#ifndef WIN32
#include <spdlog/sinks/syslog_sink.h>
#endif
#include <spdlog/sinks/stdout_color_sinks.h>


void writePidToFile(const std::string& filePath) {
    std::ofstream pidFile(filePath);
    if (pidFile.is_open()) {
        pidFile << getpid();
        pidFile.close();
    } else {
        spdlog::error("Unable to open file: {}", filePath);
    }
}

/**
 * @brief The main function of the program.
 */
int main(int argc, char* argv[]) {
    // initialize logging
    //
    // auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/switch_device.log", true);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#ifndef WIN32
    auto syslog_sink = std::make_shared<spdlog::sinks::syslog_sink_mt>("yaha", LOG_PID, LOG_USER, true);
    spdlog::sinks_init_list sink_list = { syslog_sink, console_sink };
#else
    spdlog::sinks_init_list sink_list = { console_sink };
#endif
    writePidToFile("/tmp/yaha.pid");

    auto logger = std::make_shared<spdlog::logger>("yaha", sink_list.begin(), sink_list.end());
    logger->set_pattern("%H:%M:%S %L %^%v%$");
    spdlog::set_default_logger(logger);

    // read filenames from command line
    std::string devicesFile = "devices.json";
    std::string automationFile = "automation.json";

    if (argc > 1) {
        devicesFile = argv[1];

        if (devicesFile == "create") {
            sqlite3* db = data::createDatabase("data_yaha.db");
            data::SourceSqlite source(db);
            data::TableHistory tableHistory;

            source.createTable(tableHistory);
            return 0;
        }
    }
    if (argc > 2) {
        automationFile = argv[2];
    }

    // database initialization
    //
    sqlite3* db = data::createDatabase("data_yaha.db");
    data::SourceSqlite source(db);
    data::TableHistory tableHistory;

    source.createSql(tableHistory);

    // initialize system
    //
    auto evBus = std::make_shared<EventBus>();
    auto mqtt = std::make_shared<Mqtt>(devicesFile, evBus);
    automation::Registry automations(mqtt, evBus);
    DebugOutput debugOutput;
    History history(source);

    automations.load(automationFile, evBus);

    debugOutput.registerEvents(evBus);
    history.registerEvents(evBus);

    Application app;
    TimerSlow timer1(evBus, app);
    TimerFast timer2(evBus, app);

    app.run();

    return 0;
}
