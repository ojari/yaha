#include "sourcesqlite.hpp"
#include <spdlog/spdlog.h>
#include <memory>

/*std::unique_ptr<Database> createDatabase(const std::string& filename) {
    sqlite3* db;

    sqlite3_open(filename.c_str(), &db);
    return std::unique_ptr<Database>(new Database(db));
}*/

sqlite3* createDatabase(const std::string& filename) {
    sqlite3* db;
    int result;

    if (filename.empty()) {
        result = sqlite3_open(":memory:", &db);
    } else {
        result = sqlite3_open(filename.c_str(), &db);
    }
    if (result != SQLITE_OK) {
        const char* errorString = sqlite3_errmsg(db);
        spdlog::error("sqlite open database: {}", errorString);
        sqlite3_close(db);

        return nullptr;  // or throw an exception with the error message
    }
    return db;
}
