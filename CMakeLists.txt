cmake_minimum_required(VERSION 3.20)

project(yaha)

# Use CMAKE_BUILD_TYPE
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

include(CTest)

add_subdirectory(mqtt)
#add_subdirectory(asus)
add_subdirectory(weather)
#add_subdirectory(spot)

enable_testing()
add_subdirectory(test)
