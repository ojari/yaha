#include "catch2/catch_all.hpp"
#include "../mqtt/tasks.hpp"

class TestObserver : public Observer {
public:
    void onChange(const IValueItem& value) override {
        item.set(value);
    }
    ValueItem item;
};


TEST_CASE("Collect class tests") {
    Values collect;
    TestObserver observer;

    collect.subscribe(observer);

    SECTION("Feed temperature") {
        float temperature = 25.5f;
        collect.set(ValueType::TEMPERATURE, temperature);
        REQUIRE(observer.values.getFloat(ValueType::TEMPERATURE) == temperature);

        temperature = 20.5;
        collect.set(ValueType::TEMPERATURE, temperature);
        REQUIRE(observer.values.getFloat(ValueType::TEMPERATURE) == temperature);
    }

    SECTION("Feed electricity price") {
        int elPrice = 10;
        collect.set(ValueType::ELECTRICITY_PRICE, elPrice);

        REQUIRE(observer.values.getInt(ValueType::ELECTRICITY_PRICE) == elPrice);

        elPrice = 32;
        collect.set(ValueType::ELECTRICITY_PRICE, elPrice);
        REQUIRE(observer.values.getInt(ValueType::ELECTRICITY_PRICE) == elPrice);
    }

    SECTION("Feed time") {
        int hour = 12;
        int minute = 30;
        collect.set(ValueType::TIME, hm2time(hour, minute));
        REQUIRE(observer.values.getInt(ValueType::TIME) == hm2time(hour, minute));

        hour = 8;
        minute = 59;
        collect.set(ValueType::TIME, hm2time(hour, minute));
        REQUIRE(observer.values.getInt(ValueType::TIME) == hm2time(hour, minute));
    }
}
