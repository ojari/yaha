#include "catch2/catch_all.hpp"
#include "../mqtt/data/tables.hpp"

TEST_CASE("DataValue Test", "[DataValue]") {
    DataValue value1("name1", 10);
    REQUIRE(value1.getName() == "name1");
    REQUIRE(value1.getValue<int>() == 10);

    DataValue value2("name2", 3.14f);
    REQUIRE(value2.getName() == "name2");
    REQUIRE(value2.getValue<float>() == 3.14f);

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
