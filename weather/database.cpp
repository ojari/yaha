#include "data/sourcesqlite.hpp"
#include "data/tables.hpp"


void createWeather(const std::string& filename) {
    sqlite3* db = createDatabase(filename);
    SourceSqlite source(db);
    TableWeather table;

    source.createTable(table);
}