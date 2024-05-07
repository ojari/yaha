#include "database.hpp"
#include "../common.hpp"

std::unique_ptr<Database> createDatabase(const std::string& filename) {
    sqlite3* db;

    sqlite3_open(filename.c_str(), &db);
    return std::unique_ptr<Database>(new Database(db));
}

void Database::createTable(const char* sql) {
    int rc = sqlite3_exec(db, sql, 0, 0, 0);
    if (rc != SQLITE_OK) {
        showError(sqlite3_errstr(rc));
    }
}
