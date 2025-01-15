#pragma once
#include <uv.h>
#include <spdlog/spdlog.h>
#include "hardware_info.hpp"
#include "config.hpp"


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

    virtual void onTimer() = 0;

    virtual ~TimerBase() {
        uv_timer_stop(&timer);
    }

protected:
    uv_timer_t timer;
};

//-----------------------------------------------------------------------------
// 5 minutes timer
//
struct TimerSlow : public TimerBase {
    TimerSlow(Application& app) :
        TimerBase(app, SLOW_TIMER_INTERVAL)
    {
        timer.data = this;
    }

    void onTimer() override {
        loadAvgReader.Read();
        memUsageReader.Read();
        procMemReader.Read();

        /*spdlog::info("Load: {}, proc mem: {}, Memory: {} / {}", 
            loadAvgReader.getLoad(), 
            procMemReader.getVmRSS(),
            memUsageReader.getUsedMem()/1024,
            memUsageReader.getTotalMem()/1024); */
    }

private:
    LoadAverageReader loadAvgReader;
    MemoryUsageReader memUsageReader;
    ProcessMemoryReader procMemReader;
};

//-----------------------------------------------------------------------------
// One minute timer
//
struct TimerFast : public TimerBase {
    TimerFast(Application& app, TaskManager& taskManager) :
        TimerBase(app, FAST_TIMER_INTERVAL),
        taskManager(taskManager)
    {
        timer.data = this;
    }

    void onTimer() override {
        taskManager.execute();
    }

private:
    TaskManager &taskManager;
};

