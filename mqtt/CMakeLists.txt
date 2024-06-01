include(../setup.cmake)
cmake_minimum_required(VERSION 3.10)

project(yaha)

# specify the C++ standard
# set(CMAKE_CXX_STANDARD 17)
# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# add the executable
add_executable(${PROJECT_NAME}
  main.cpp
  mqtt.cpp
  mqtt_device.cpp
  common.cpp
  actuator.cpp
  logic.cpp
  collect.cpp
  data/database.cpp
)


target_include_directories(${PROJECT_NAME} PRIVATE 
    ${VCPKG_ROOT}/include
)
target_link_directories(${PROJECT_NAME} PRIVATE
    ${VCPKG_ROOT}/lib
)
target_link_libraries(${PROJECT_NAME}
    ${MOSQUITTO_LIB}
    ${EXTRA_LIBRARIES}
    sqlite3
 #   redis++
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
