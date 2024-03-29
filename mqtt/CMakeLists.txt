
if (WIN32)
  set(VCPKG_DIR c:/usr/vcpkg)
  set(VCPKG_ROOT ${VCPKG_DIR}/installed/x64-windows)
  set(CURL_LIB libcurl)
  set(EXTRA_LIBRARIES libcrypto libssl)
else()
  set(VCPKG_DIR /home/jari/vcpkg-2024.02.14)
  set(VCPKG_ROOT ${VCPKG_DIR}/installed/x64-linux)
  set(CURL_LIB curl)
  set(EXTRA_LIBRARIES ssl crypto pthread dl z)
endif()

set(CMAKE_TOOLCHAIN_FILE ${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake)

cmake_minimum_required(VERSION 3.10)

# set the project name
project(ExpertSystem)

find_package(mosquitto REQUIRED)

# specify the C++ standard
# set(CMAKE_CXX_STANDARD 17)
# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# add the executable
add_executable(${PROJECT_NAME}
  expert.cpp
  main.cpp
  mqtt.cpp
  scheduler.cpp
)


target_include_directories(${PROJECT_NAME} PRIVATE 
    ${VCPKG_ROOT}/include
)
target_link_directories(${PROJECT_NAME} PRIVATE
    ${VCPKG_ROOT}/lib
)
target_link_libraries(${PROJECT_NAME}
    mosquitto
)
