#define CATCH_CONFIG_RUNNER // Configure catch to use your main, and not its own.
#include "catch2/catch_all.hpp"
#include <iostream>
#include <exception>
#include <filesystem>

void remove_database() {
    std::cout << "Removing database\n";
    try {
        std::filesystem::remove("test.db");
    }
    catch (const std::exception& ex) {
        std::cout << "An exception was thrown: " << ex.what();
    }
}

int main(int argCount, char** ppArgs) {
    // remove_database();
    try {
        auto result = Catch::Session().run(argCount, ppArgs);
        if (result != 0) {
            std::cout << "Result: " << result << "\n";
        }
        return (result < 0xFF ? result : 0xFF);
    }
    catch (const std::exception& ex) {
        auto pMessage = ex.what();
        if (pMessage) {
            std::cout << "An unhandled exception was thrown:\n" << pMessage;
        }
        return -1;
    }
}
