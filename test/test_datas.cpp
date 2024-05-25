#include "catch2/catch_all.hpp"
#include "../mqtt/data/datas.hpp"

TEST_CASE("DataValue Test", "[DataValue]") {
    DataValue value1("name1", 10);
    REQUIRE(value1.getName() == "name1");
    REQUIRE(value1.getValue<int>() == 10);

    DataValue value2("name2", 3.14);
    REQUIRE(value2.getName() == "name2");
    REQUIRE(value2.getValue<double>() == 3.14);

    DataValue value3("name3", "hello");
    REQUIRE(value3.getName() == "name3");
    REQUIRE(value3.getValue<std::string>() == "hello");
}

TEST_CASE("TableConfigDevice Test", "[TableConfigDevice]") {
    TableConfigDevice table;

    ConfigDevice config("device1", "type1");
    table.set(config);

    REQUIRE(table.getConfig().name == "device1");
    REQUIRE(table.getConfig().type == "type1");
}

TEST_CASE("TableConfigController Test", "[TableConfigController]") {
    TableConfigController table;

    ConfigController config("controller1", "type1", "actuator1", 10, 20);
    table.set(config);

    REQUIRE(table.getConfig().name == "controller1");
    REQUIRE(table.getConfig().type == "type1");
    REQUIRE(table.getConfig().actuator == "actuator1");
    REQUIRE(table.getConfig().time1 == 10);
    REQUIRE(table.getConfig().time2 == 20);
}

TEST_CASE("SourceString Test", "[SourceString]") {
    SourceString<ConfigDevice> source("foo bar\none two\n");
    TableConfigDevice tableDevice;

    source.read(tableDevice);
    auto device = tableDevice.getConfig();
    REQUIRE(device.name == "foo");
    REQUIRE(device.type == "bar");

    source.read(tableDevice);
    device = tableDevice.getConfig();
    REQUIRE(device.name == "one");
    REQUIRE(device.type == "two");
}

TEST_CASE("SourceString 2nd Test", "[SourceString]") {
    // Existing tests...

    SECTION("Read multiple lines from source") {
        SourceString<ConfigDevice> source("device1 type1\ncontroller1 type2 actuator1 10 20\n");
        TableConfigDevice tableDevice;
        source.read(tableDevice);
        auto device = tableDevice.getConfig();
        REQUIRE(device.name == "device1");
        REQUIRE(device.type == "type1");

        TableConfigController tableController;
        source.read(tableController);
        auto controller = tableController.getConfig();
        REQUIRE(controller.name == "controller1");
        REQUIRE(controller.type == "type2");
        REQUIRE(controller.actuator == "actuator1");
        REQUIRE(controller.time1 == 10);
        REQUIRE(controller.time2 == 20);
    }
}
