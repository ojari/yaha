include(../setup.cmake)
cmake_minimum_required(VERSION 3.20)
project(weather VERSION 0.1)

add_executable(${PROJECT_NAME}
  main.cpp
  parse.cpp
  ../common/utils.cpp
)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(${PROJECT_NAME} PRIVATE
  ${VCPKG_ROOT}/include
)
target_link_directories(${PROJECT_NAME} PRIVATE
    ${VCPKG_ROOT}/lib
)
target_link_libraries(${PROJECT_NAME}
  tinyxml2
  ${EXTRA_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)