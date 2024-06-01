cmake_minimum_required(VERSION 3.20)

project(yaha)
add_subdirectory(mqtt)

project(weather)
add_subdirectory(weather)

enable_testing()
project(test)
add_subdirectory(test)