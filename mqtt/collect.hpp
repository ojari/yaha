#pragma once

#include "events.hpp"
#include "common.hpp"
#include <memory>

class BaseSource {
public:
    BaseSource(std::shared_ptr<IValues> collect) : 
        collect(collect)
    {}
protected:
    std::shared_ptr<IValues> collect;
};


class SourceTime : public BaseSource, public ITask {
public:
    SourceTime(std::shared_ptr<IValues> collect) : BaseSource(collect)
    {}

    void execute() {
        incrementTime(1);
        int time = hm2time(hour, minute);
        collect->set(ValueType::TIME, time);
    }
private:
    void incrementTime(int minutes);

    int hour {0};
    int minute {0};
};

class SourceTemperature : public BaseSource, public ITask {
public:
    SourceTemperature(std::shared_ptr<IValues> collect) : BaseSource(collect)
    {}

    void execute() {
        if (counter < 10) {
            collect->set(ValueType::TEMPERATURE, 20.0f);
        }
        else if (counter < 20) {
            collect->set(ValueType::TEMPERATURE, 21.0f);
        }
        else {
            counter = 0;
        }
        counter++;
    }
private:
    int counter {0};
};
