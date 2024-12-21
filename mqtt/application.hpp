#pragma once
#include <uv.h>
#include <spdlog/spdlog.h>
#include "hardware_info.hpp"


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
struct TimerSlow : public TimerBase {
    TimerSlow(Application& app) :
        TimerBase(app, 5*60000)
    {
        timer.data = this;
    }

    void onTimer() override {
        //counter++;
        //if (counter % 10 == 0) {
            loadAvgReader.Read();
            memUsageReader.Read();
            procMemReader.Read();

            spdlog::info("Load: 1m: {}, proc mem: {}, Memory total: {}, used: {}", 
                loadAvgReader.getLoad(), 
                procMemReader.getVmRSS(),
                memUsageReader.getTotalMem()/1024,
                memUsageReader.getUsedMem()/1024);
        //}
    }

private:
    LoadAverageReader loadAvgReader;
    MemoryUsageReader memUsageReader;
    ProcessMemoryReader procMemReader;

    int counter = 0;
};

//-----------------------------------------------------------------------------
struct TimerFast : public TimerBase {
    TimerFast(Application& app) :
        TimerBase(app, 1000)
    {
        timer.data = this;
    }

    void onTimer() override {
        counter++;
        if (counter % 4 == 0) {
            //spdlog::info("TimerFast b: {}", counter);
        }
    }

private:
    int counter = 0;
};
