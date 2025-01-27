include(../setup.cmake)
cmake_minimum_required(VERSION 3.20)

project(weather VERSION 0.1)

add_executable(${PROJECT_NAME}
  main.cpp
  parse.cpp
  database.cpp
  ../common/utils.cpp
  ../mqtt/data/sourcesqlite.cpp
  ../mqtt/data/tables.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(${PROJECT_NAME} PRIVATE
  ${VCPKG_ROOT}/include
  ../mqtt
)
target_link_directories(${PROJECT_NAME} PRIVATE
    ${VCPKG_ROOT}/lib
)
target_link_libraries(${PROJECT_NAME}
  tinyxml2 z
  ${CURL_LIB}
  ${EXTRA_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
