#pragma once

#include "events.hpp"
#include "common.hpp"
#include <memory>

/** Collects temperature, elprice and time from sources and publish result into observers.*/
class Collect : public Observable {
public:
    void feedTemperature(float temperature);
    void feedElPrice(int elPrice);
    void feedTime(int hour, int minute);

private:
    Values values;
};


class BaseSource {
public:
    BaseSource(std::shared_ptr<Collect> collect) : 
        collect(collect)
    {}
protected:
    std::shared_ptr<Collect> collect;
};


class SourceTime : public BaseSource, public Task {
public:
    SourceTime(std::shared_ptr<Collect> collect) : BaseSource(collect)
    {}

    void execute() {
        incrementTime(1);
        collect->feedTime(hour, minute);
    }
private:
    void incrementTime(int minutes);

    int hour {0};
    int minute {0};
};

class SourceTemperature : public BaseSource, public Task {
public:
    SourceTemperature(std::shared_ptr<Collect> collect) : BaseSource(collect)
    {}

    void execute() {
        if (counter < 10) {
            collect->feedTemperature(20.0);
        }
        else if (counter < 20) {
            collect->feedTemperature(21.0);
        }
        else {
            counter = 0;
        }
        counter++;
    }
private:
    int counter {0};
};
