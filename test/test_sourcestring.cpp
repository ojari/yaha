#include <gtest/gtest.h>

#include "../mqtt/data/tables.hpp"
#include "../mqtt/data/sourcestring.hpp"


TEST(SourceStringTest, ReadsSingleLine) {
    SourceString<ConfigDevice> source("{ device1 type1 }\n");
    TableConfigDevice tableDevice;
    ConfigDevice device;

    source.read(tableDevice);
    dataFromHeader(tableDevice, device);

    EXPECT_EQ(device.name, "device1");
    EXPECT_EQ(device.type, "type1");
}

TEST(SourceStringTest, InsertsSingleLine) {
    SourceString<ConfigDevice> source("");
    TableConfigDevice tableDevice;
    ConfigDevice config("device2", "type2");
    dataToHeader(tableDevice, config);
    source.insert(tableDevice);

    TableConfigDevice tableDevice2;
    source.read(tableDevice2);
    dataFromHeader(tableDevice2, config);
    EXPECT_EQ(config.name, "device2");
    EXPECT_EQ(config.type, "type2");
}

TEST(SourceStringTest, TracksEndOfFileState) {
    SourceString<ConfigDevice> source(
        "{ device1 type1 }\n"
        "{ device2 type2 }");
    TableConfigDevice tableDevice;

    EXPECT_FALSE(source.isEnd());
    source.read(tableDevice);
    EXPECT_FALSE(source.isEnd());
    source.read(tableDevice);
    EXPECT_TRUE(source.isEnd());
}

TEST(SourceStringTest, ReadsMultipleLines) {
    SourceString<ConfigDevice> source("{ device1 type1 }\n{ controller1 type2 actuator1 10 20 }\n");
    TableConfigDevice tableDevice;
    ConfigDevice device;

    source.read(tableDevice);
    dataFromHeader(tableDevice, device);

    EXPECT_EQ(device.name, "device1");
    EXPECT_EQ(device.type, "type1");

    TableConfigController tableController;
    source.read(tableController);
    ConfigController controller;
    dataFromHeader(tableController, controller);

    EXPECT_EQ(controller.name, "controller1");
    EXPECT_EQ(controller.type, "type2");
    EXPECT_EQ(controller.actuator, "actuator1");
    EXPECT_EQ(controller.time1, 10);
    EXPECT_EQ(controller.time2, 20);
}


