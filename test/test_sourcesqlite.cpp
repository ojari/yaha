#include <gtest/gtest.h>

#include "../mqtt/data/tables.hpp"
#include "../mqtt/data/sourcesqlite.hpp"

namespace {
void closeDb(sqlite3* db) {
    sqlite3_close(db);
}
}

TEST(SourceSqliteTest, CreatesSqlStatementsForDeviceTable) {
    sqlite3* db = data::createDatabase("");
    data::SourceSqlite source(db);
    data::TableConfigDevice tableDevice;

    EXPECT_EQ(source.createSql(tableDevice), "CREATE TABLE IF NOT EXISTS device(name TEXT,type TEXT);");
    EXPECT_EQ(source.insertSql(tableDevice), "INSERT INFO device(name,type ) VALUES (?,?);");
    EXPECT_EQ(source.selectSql(tableDevice), "SELECT name,type FROM device;");

    closeDb(db);
}

TEST(SourceSqliteTest, InsertSingleRowIntoControllerTable) {
    sqlite3* db = data::createDatabase("");
    data::SourceSqlite source(db);
    data::TableConfigController tableController;
    data::ConfigController config("controller2", "type2", "actuator2", 30, 40);
    dataToHeader(tableController, config);

    EXPECT_NO_THROW(source.insert(tableController));

    closeDb(db);
}

TEST(SourceSqliteTest, InsertAndReadDeviceRow) {
    sqlite3* db = data::createDatabase("");
    data::SourceSqlite source(db);
    data::TableConfigDevice tableDevice;
    data::ConfigDevice config("device1", "type1");
    dataToHeader(tableDevice, config);

    source.insert(tableDevice);

    auto iter = source.begin(tableDevice);
    (void)iter;

    closeDb(db);
}
