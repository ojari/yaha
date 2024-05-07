#include "database.hpp"

std::unique_ptr<Database> createDatabase(const std::string& filename) {
    sqlite3* db;

    sqlite3_open(filename.c_str(), &db);
    return std::unique_ptr<Database>(new Database(db));
}
