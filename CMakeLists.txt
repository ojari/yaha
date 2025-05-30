cmake_minimum_required(VERSION 3.20)

project(yaha)

option(COVERAGE "Enable coverage reporting" OFF)

if(COVERAGE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(--coverage)
    add_link_options(--coverage)
  else()
    message(WARNING "Coverage option is only supported with GCC or Clang.")
  endif()
endif()

# Use CMAKE_BUILD_TYPE
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

include(CTest)

add_subdirectory(mqtt)
#add_subdirectory(asus)
add_subdirectory(weather)
#add_subdirectory(spot)

#enable_testing()
#add_subdirectory(test)
