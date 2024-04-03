#include <iostream>

void showError(const char* message) {
    std::cerr << "ERROR: " << message << std::endl;
}

void showError(const char* message, const char* arg) {
    std::cerr << "ERROR: " << message << " " << arg << std::endl;
}
