#pragma once
#include <uv.h>
#include <spdlog/spdlog.h>
#include "task/load_average_reader.hpp"
#include "task/memory_usage_reader.hpp"
#include "task/process_memory_reader.hpp"
#include "config.hpp"

#include "task/temperature.hpp"
#include "task/time.hpp"
#include "task/calc_price.hpp"
#include "task/calc_sun.hpp"

struct Application {
    Application() {
        uv_loop_init(&loop);
    }

    ~Application() {
        uv_loop_close(&loop);
    }

    void run() {
        uv_run(&loop, UV_RUN_DEFAULT);
    }

    uv_loop_t* getLoop() {
        return &loop;
    }

private:
    uv_loop_t loop;
};

//-----------------------------------------------------------------------------
struct TimerBase {
    TimerBase(Application& app, int interval) {
        uv_timer_init(app.getLoop(), &timer);
        uv_timer_start(&timer, [](uv_timer_t* handle) {
            TimerBase* timer = static_cast<TimerBase*>(handle->data);
            timer->onTimer();
        }, interval, interval);
        timer.data = this;
    }

    void onTimer() {
        for (auto& task : tasks) {
            try {
                task->execute();
            }
            catch (const std::exception& ex) {
                spdlog::error("TimerSlow task execution failed: {}", ex.what());
            }
        }
    }

    virtual ~TimerBase() {
        uv_timer_stop(&timer);
    }

protected:
    uv_timer_t timer;
    std::vector<std::unique_ptr<ITask>> tasks;
};

//-----------------------------------------------------------------------------
// 5 minutes timer
//
struct TimerSlow : public TimerBase {
    explicit TimerSlow(EventBus& evbus, Application& app) :
        TimerBase(app, SLOW_TIMER_INTERVAL)
    {
        timer.data = this;
        // Fill the tasks vector with the required ITask implementations
        tasks.emplace_back(std::make_unique<task::LoadAverageReader>());
        tasks.emplace_back(std::make_unique<task::MemoryUsageReader>());
        tasks.emplace_back(std::make_unique<task::ProcessMemoryReader>(evbus));

        tasks.emplace_back(std::make_unique<task::TaskCalcSun>(evbus));
    }
};

//-----------------------------------------------------------------------------
// One minute timer
//
struct TimerFast : public TimerBase {
    TimerFast(EventBus& evbus, Application& app) :
        TimerBase(app, FAST_TIMER_INTERVAL)
    {
        timer.data = this;

        tasks.emplace_back(std::make_unique<task::TaskTemperature>(evbus));
#ifdef DEBUG_TIME
        tasks.emplace_back(std::make_unique<task::TaskDebugTime>(evbus));
#else
        tasks.emplace_back(std::make_unique<task::TaskTime>(evbus));
#endif
        tasks.emplace_back(std::make_unique<task::TaskCalcPrice>(evbus));

        for (const auto& task : tasks) {
            task->initialize();
        }
    }
};

