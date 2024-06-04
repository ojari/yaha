#include "catch2/catch_all.hpp"
#include "../mqtt/data/tables.hpp"
#include "../mqtt/data/sourcestring.hpp"


TEST_CASE("SourceString Test", "[SourceString]") {
    SECTION("Read single line from source") {
        SourceString<ConfigDevice> source("device1 type1\n");
        TableConfigDevice tableDevice;
        source.read(tableDevice);
        auto device = tableDevice.get();
        REQUIRE(device.name == "device1");
        REQUIRE(device.type == "type1");
    }
    SECTION("Insert single line to source") {
        SourceString<ConfigDevice> source("");
        TableConfigDevice tableDevice;
        ConfigDevice config("device2", "type2");
        tableDevice.set(config);
        source.insert(tableDevice);

        TableConfigDevice tableDevice2;
        source.read(tableDevice2);
        auto device = tableDevice2.get();
        REQUIRE(device.name == "device2");
        REQUIRE(device.type == "type2");
    }
    SECTION("Check isEof method") {
        SourceString<ConfigDevice> source(
            "device1 type1\n"
            "device2 type2");  // No newline at the end
        TableConfigDevice tableDevice;
        REQUIRE(source.isEnd() == false);
        source.read(tableDevice);
        REQUIRE(source.isEnd() == false);
        source.read(tableDevice);
        REQUIRE(source.isEnd() == true);
    }
}

TEST_CASE("SourceString 2nd Test", "[SourceString]") {
    // Existing tests...

    SECTION("Read multiple lines from source") {
        SourceString<ConfigDevice> source("device1 type1\ncontroller1 type2 actuator1 10 20\n");
        TableConfigDevice tableDevice;
        source.read(tableDevice);
        auto device = tableDevice.get();
        REQUIRE(device.name == "device1");
        REQUIRE(device.type == "type1");

        TableConfigController tableController;
        source.read(tableController);
        auto controller = tableController.get();
        REQUIRE(controller.name == "controller1");
        REQUIRE(controller.type == "type2");
        REQUIRE(controller.actuator == "actuator1");
        REQUIRE(controller.time1 == 10);
        REQUIRE(controller.time2 == 20);
    }
}

