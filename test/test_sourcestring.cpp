#include "catch2/catch_all.hpp"
#include "../mqtt/data/tables.hpp"
#include "../mqtt/data/sourcestring.hpp"


TEST_CASE("SourceString Test", "[SourceString]") {
    SECTION("Read single line from source") {
        SourceString<ConfigDevice> source("device1 type1\n");
        TableConfigDevice tableDevice;
        ConfigDevice device;

        source.read(tableDevice);
        dataFromHeader(tableDevice, device);

        REQUIRE(device.name == "device1");
        REQUIRE(device.type == "type1");
    }
    SECTION("Insert single line to source") {
        SourceString<ConfigDevice> source("");
        TableConfigDevice tableDevice;
        ConfigDevice config("device2", "type2");
        dataToHeader(tableDevice, config);
        source.insert(tableDevice);

        TableConfigDevice tableDevice2;
        source.read(tableDevice2);
        dataFromHeader(tableDevice2, config);
        REQUIRE(config.name == "device2");
        REQUIRE(config.type == "type2");
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
        ConfigDevice device;

        source.read(tableDevice);
        dataFromHeader(tableDevice, device);

        REQUIRE(device.name == "device1");
        REQUIRE(device.type == "type1");

        TableConfigController tableController;
        source.read(tableController);
        ConfigController controller;
        dataFromHeader(tableController, controller);

        REQUIRE(controller.name == "controller1");
        REQUIRE(controller.type == "type2");
        REQUIRE(controller.actuator == "actuator1");
        REQUIRE(controller.time1 == 10);
        REQUIRE(controller.time2 == 20);
    }
}

