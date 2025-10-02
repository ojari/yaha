
include(../setup.cmake)

cmake_minimum_required(VERSION 3.10)

project(utest)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# add the executable
add_executable(${PROJECT_NAME}
  mocks.h
  main.cpp
  # test_orm.cpp
  test_logic.cpp
  # test_collect.cpp
  test_time.cpp
  test_datas.cpp
  test_shelly.cpp
  test_light_device.cpp
  test_switch_device.cpp
  test_task_calc_sun_reference.cpp
  #test_sourcesqlite.cpp
  #test_sourcestring.cpp

  ../mqtt/common.cpp
  ../mqtt/common.hpp
  ../mqtt/demo/suntime.cpp
  ../mqtt/demo/sun_data.cpp
  ../mqtt/task/time.cpp
  ../mqtt/task/calc_sun.hpp
  ../mqtt/automation/lights.cpp
  ../mqtt/automation/switch_light.cpp
  ../mqtt/automation/car_heater.cpp
  ../mqtt/automation/automation.cpp
  #../mqtt/data/sourcesqlite.cpp
  ../mqtt/data/sourcestring.cpp
  ../mqtt/data/tables.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${VCPKG_ROOT}/include
    ../mqtt
)
target_link_directories(${PROJECT_NAME} PRIVATE
    ${VCPKG_ROOT}/lib
    ${VCPKG_ROOT}/debug/lib
    ${VCPKG_ROOT}/lib/manual-link
)
target_link_libraries(${PROJECT_NAME}
    Catch2d
    ${EXTRA_LIBRARIES}
)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
