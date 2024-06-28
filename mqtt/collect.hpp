#pragma once

#include "events.hpp"
#include "common.hpp"
#include <memory>

class BaseSource {
public:
    explicit BaseSource(std::shared_ptr<IValues> collect) : 
        collect(collect)
    {}
protected:
    void set(ValueType name, float value) const {
        collect->set(name, value);
    }
    void set(ValueType name, int value) const {
        collect->set(name, value);
    }
private:
    std::shared_ptr<IValues> collect;
};


class SourceTime : public BaseSource, public ITask {
public:
    explicit SourceTime(std::shared_ptr<IValues> collect) : BaseSource(collect)
    {}

    void execute() override {
        incrementTime(1);
        int time = hm2time(hour, minute);
        set(ValueType::TIME, time);
    }
private:
    void incrementTime(int minutes);

    int hour {0};
    int minute {0};
};

class SourceTemperature : public BaseSource, public ITask {
public:
    explicit SourceTemperature(std::shared_ptr<IValues> collect) : BaseSource(collect)
    {}

    void execute() override {
        if (counter < 10) {
            set(ValueType::TEMPERATURE, 20.0f);
        }
        else if (counter < 20) {
            set(ValueType::TEMPERATURE, 21.0f);
        }
        else {
            counter = 0;
        }
        counter++;
    }
private:
    int counter {0};
};
