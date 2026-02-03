#include <exception>
#include <filesystem>
#include <iostream>

#include <gtest/gtest.h>

void remove_database() {
    std::cout << "Removing database\n";
    try {
        std::filesystem::remove("test.db");
    }
    catch (const std::exception& ex) {
        std::cout << "An exception was thrown: " << ex.what();
    }
}

int main(int argc, char** argv) {
    // remove_database();
    try {
        ::testing::InitGoogleTest(&argc, argv);
        const int result = RUN_ALL_TESTS();
        if (result != 0) {
            std::cout << "Result: " << result << "\n";
        }
        return result;
    }
    catch (const std::exception& ex) {
        if (const char* message = ex.what()) {
            std::cout << "An unhandled exception was thrown:\n" << message;
        }
        return -1;
    }
}
