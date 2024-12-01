
include(../setup.cmake)

cmake_minimum_required(VERSION 3.10)

project(utest)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# add the executable
add_executable(${PROJECT_NAME}
  main.cpp
  # test_orm.cpp
  test_logic.cpp
  # test_collect.cpp
  test_time.cpp
  test_datas.cpp
  test_sourcesqlite.cpp
  test_sourcestring.cpp

  ../mqtt/common.cpp
  ../mqtt/task/time.cpp
  ../mqtt/automation/lights.cpp
  ../mqtt/automation/car_heater.cpp
  ../mqtt/automation/automation.cpp
  ../mqtt/data/sourcesqlite.cpp
  ../mqtt/data/sourcestring.cpp
  ../mqtt/data/tables.cpp
)


target_include_directories(${PROJECT_NAME} PRIVATE 
    ${VCPKG_ROOT}/include
    ../mqtt
)
target_link_directories(${PROJECT_NAME} PRIVATE
    ${VCPKG_ROOT}/lib
    ${VCPKG_ROOT}/lib/manual-link
)
target_link_libraries(${PROJECT_NAME}
    Catch2
    ${MOSQUITTO_LIB}
    sqlite3
    ${EXTRA_LIBRARIES}
)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
