include(../setup.cmake)
cmake_minimum_required(VERSION 3.10)

project(yaha)

# Making flatbuffers
#

# Specify the directories where the .fbs files are located
set(FBS_SRC_DIR ${CMAKE_SOURCE_DIR}/etc)
set(FBS_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(FLATC ${VCPKG_TOOLS}/flatbuffers/flatc)

set(FBS_FILES
   # ${FBS_SRC_DIR}/fbsdata.fbs
   # ${FBS_SRC_DIR}/elprice.fbs
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
  history.cpp
  task_manager.cpp
  automation/registry.cpp
  automation/automation.cpp
  automation/lights.cpp
  automation/car_heater.cpp
  automation/room_heater.cpp
  automation/storage_heater.cpp
  automation/switch_light.cpp
  automation/water_heater.cpp
  task/time.cpp
  data/sourcesqlite.cpp
  data/tables.cpp
  device/registry.cpp
  ${GENERATED_HEADERS}
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
    ${MQTT_LIB}
    ${EXTRA_LIBRARIES}
    sqlite3
 #   redis++
)

if(DEFINED DEBUG_TIME)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_TIME)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
