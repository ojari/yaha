include(../setup.cmake)
cmake_minimum_required(VERSION 3.10)

project(yaha)

# Making flatbuffers
#

# Specify the directories where the .fbs files are located
set(FBS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(FBS_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(FLATC ${VCPKG_ROOT}/tools/flatbuffers/flatc)

set(FBS_FILES
    ${FBS_SRC_DIR}/fbsdata.fbs
)

# Add custom command to compile .fbs files
foreach(FBS_FILE ${FBS_FILES})
    get_filename_component(FBS_NAME ${FBS_FILE} NAME_WE)
    add_custom_command(
        OUTPUT  ${FBS_BIN_DIR}/${FBS_NAME}_generated.h
        COMMAND ${FLATC} -c -o ${FBS_BIN_DIR} ${FBS_FILE}
        DEPENDS ${FBS_FILE}
        COMMENT "Building C++ header from ${FBS_FILE}"
    )
    list(APPEND GENERATED_HEADERS ${FBS_BIN_DIR}/${FBS_NAME}_generated.h)
endforeach()


# Compile application
#

add_executable(${PROJECT_NAME}
  main.cpp
  mqtt.cpp
  common.cpp
  actuator.cpp
  controller/registry.cpp
  controller/boolean_controller.cpp
  controller/lights.cpp
  controller/car_heater.cpp
  controller/room_heater.cpp
  controller/storage_heater.cpp
  controller/water_heater.cpp
  task/time.cpp
  data/sourcesqlite.cpp
  data/tables.cpp
  device/registry.cpp
)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)


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
