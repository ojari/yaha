#include <gtest/gtest.h>
#include "../mqtt/data/tables.hpp"

TEST(DataTablesTest, DataValue) {
    data::DataValue value1("name1", 10);
    EXPECT_EQ(value1.getName(), "name1");
    EXPECT_EQ(value1.getValue<int>(), 10);

    data::DataValue value2("name2", 3.14f);
    EXPECT_EQ(value2.getName(), "name2");
    EXPECT_FLOAT_EQ(value2.getValue<float>(), 3.14f);

    data::DataValue value3("name3", "hello");
    EXPECT_EQ(value3.getName(), "name3");
    EXPECT_EQ(value3.getValue<std::string>(), "hello");
}

TEST(DataTablesTest, TableConfigDevice) {
    data::TableConfigDevice table;

    data::ConfigDevice config("device1", "type1");
    dataToHeader(table, config);

    const std::string name = table.getValue<std::string>("name");
    EXPECT_EQ(name, "device1");

    std::string type = table.getValue<std::string>("type");
    EXPECT_EQ(type, "type1");
}

TEST(DataTablesTest, TableConfigController) {
    data::TableConfigController table;

    data::ConfigController config("controller1", "type1", "actuator1", 10, 20);
    dataToHeader(table, config);

    EXPECT_EQ(table.getValue<std::string>("name"), "controller1");
    EXPECT_EQ(table.getValue<std::string>("type"), "type1");
    EXPECT_EQ(table.getValue<std::string>("actuator"), "actuator1");
    EXPECT_EQ(table.getValue<int>("time1"), 10);
    EXPECT_EQ(table.getValue<int>("time2"), 20);
}
