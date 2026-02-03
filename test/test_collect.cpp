#include <gtest/gtest.h>
#include "../mqtt/tasks.hpp"

class TestObserver : public Observer {
public:
    void onChange(const IValueItem& value) override {
        item.set(value);
    }
    ValueItem item;
};

class CollectTest : public ::testing::Test {
protected:
    void SetUp() override {
        collect.subscribe(observer);
    }

    Values collect;
    TestObserver observer;
};

TEST_F(CollectTest, RecordsTemperature) {
    float temperature = 25.5f;
    collect.set(ValueType::TEMPERATURE, temperature);
    EXPECT_FLOAT_EQ(observer.values.getFloat(ValueType::TEMPERATURE), temperature);

    temperature = 20.5f;
    collect.set(ValueType::TEMPERATURE, temperature);
    EXPECT_FLOAT_EQ(observer.values.getFloat(ValueType::TEMPERATURE), temperature);
}

TEST_F(CollectTest, RecordsElectricityPrice) {
    int elPrice = 10;
    collect.set(ValueType::ELECTRICITY_PRICE, elPrice);
    EXPECT_EQ(observer.values.getInt(ValueType::ELECTRICITY_PRICE), elPrice);

    elPrice = 32;
    collect.set(ValueType::ELECTRICITY_PRICE, elPrice);
    EXPECT_EQ(observer.values.getInt(ValueType::ELECTRICITY_PRICE), elPrice);
}

TEST_F(CollectTest, RecordsTime) {
    int hour = 12;
    int minute = 30;
    collect.set(ValueType::TIME, hm2time(hour, minute));
    EXPECT_EQ(observer.values.getInt(ValueType::TIME), hm2time(hour, minute));

    hour = 8;
    minute = 59;
    collect.set(ValueType::TIME, hm2time(hour, minute));
    EXPECT_EQ(observer.values.getInt(ValueType::TIME), hm2time(hour, minute));
}

