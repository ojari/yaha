#include "catch2/catch_all.hpp"
#include "../mqtt/collect.hpp"

class TestObserver : public Observer {
public:
    void onChange(const Values& state) override {
        values = state;
    }
    Values values;
};


TEST_CASE("Collect class tests") {
    Collect collect;
    TestObserver observer;
    
    collect.subscribe(observer);

    SECTION("Feed temperature") {
        float temperature = 25.5;
        collect.feedTemperature(temperature);
        REQUIRE(observer.values.getTemperature() == temperature);

        temperature = 20.5;
        collect.feedTemperature(temperature);
        REQUIRE(observer.values.getTemperature() == temperature);
    }
    
    SECTION("Feed electricity price") {
        int elPrice = 10;
        collect.feedElPrice(elPrice);
        REQUIRE(observer.values.getElPrice() == elPrice);
        
        elPrice = 32;
        collect.feedElPrice(elPrice);
        REQUIRE(observer.values.getElPrice() == elPrice);
    }
    
    SECTION("Feed time") {
        int hour = 12;
        int minute = 30;
        collect.feedTime(hour, minute);
        REQUIRE(observer.values.getTime() == hm2time(hour, minute));

        hour = 8;
        minute = 59;
        collect.feedTime(hour, minute);
        REQUIRE(observer.values.getTime() == hm2time(hour, minute));
    }
}
