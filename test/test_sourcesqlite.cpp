#include "catch2/catch_all.hpp"
#include "../mqtt/data/tables.hpp"
#include "../mqtt/data/sourcesqlite.hpp"

void close_db(sqlite3* db) {
    sqlite3_close(db);
}

TEST_CASE("SourceSqlite Test", "[SourceSqlite]") {
    SECTION("SQL statement creation for ConfigDevice table") {
        sqlite3* db = createDatabase("");
        SourceSqlite source(db);
        TableConfigDevice tableDevice;

        REQUIRE(source.createSql(tableDevice) == "CREATE TABLE IF NOT EXISTS device(name TEXT,type TEXT);");
        REQUIRE(source.insertSql(tableDevice) == "INSERT INFO device(name,type ) VALUES (?,?);");
        REQUIRE(source.selectSql(tableDevice) == "SELECT name,type FROM device;");

        close_db(db);
    }
    SECTION("Read single row from SQLite database") {
        sqlite3* db = createDatabase("");
        SourceSqlite source(db);
        TableConfigDevice tableDevice;
        ConfigDevice device;

        // Note: must create table in memory before inserting data
        //       must add data to table before reading

        // auto iter = source.begin(tableDevice);

        // dataFromHeader(*iter, device);

        // Verify the values
        // REQUIRE(device.name == "device1");
        // REQUIRE(device.type == "type1");

        close_db(db);
    }
    SECTION("Insert single row into SQLite database") {
        sqlite3* db = createDatabase("");

        SourceSqlite source(db);
        TableConfigController tableController;
        ConfigController config("controller2", "type2", "actuator2", 30, 40);
        dataToHeader(tableController, config);
        source.insert(tableController);
/*
        // Query the database to verify the inserted row
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, "SELECT * FROM controllers;", -1, &stmt, 0);
        REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
        REQUIRE(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))) == "controller2");
        REQUIRE(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))) == "type2");
        REQUIRE(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))) == "actuator2");
        REQUIRE(sqlite3_column_int(stmt, 3) == 30);
        REQUIRE(sqlite3_column_int(stmt, 4) == 40);

        // Close the database
        sqlite3_finalize(stmt);
*/
        close_db(db);
    }
    /*SECTION("Insert single row into SQLite database") {
        sqlite3* db = create_db();

        // Create a SourceSqlite object and insert a row
        SourceSqlite<ConfigDevice> source(db, "devices");
        TableConfigDevice tableDevice;
        ConfigDevice config("device2", "type2");
        tableDevice.set(config);
        source.insert(tableDevice);

        // Query the database to verify the inserted row
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, "SELECT * FROM devices;", -1, &stmt, 0);
        REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
        REQUIRE(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))) == "device2");
        REQUIRE(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))) == "type2");

        // Close the database
        sqlite3_finalize(stmt);
        close_db(db);
    }*/
}

TEST_CASE("Insert and Read item from SQLite database") {
    sqlite3* db = createDatabase("");
    SourceSqlite source(db);
    TableConfigDevice tableDevice;
    ConfigDevice config("device1", "type1");
    dataToHeader(tableDevice, config);

    source.insert(tableDevice);

    // Read the inserted item

    auto iter = source.begin(tableDevice);
    // IDataHeader& header = *(*iter);
    // auto device = tableDevice.get();

    // Verify the values
    // REQUIRE(device.name == "device1");
    // REQUIRE(device.type == "type1");

    close_db(db);
}
