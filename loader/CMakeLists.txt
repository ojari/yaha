include(../setup.cmake)
cmake_minimum_required(VERSION 3.10)
project(loader VERSION 0.1)



# target_include_directories(${PROJECT_NAME} PRIVATE ../common)
add_executable(${PROJECT_NAME}
    ICommand.hpp
    spot_download.hpp
    spot_parse.hpp
    weather_download.hpp
    weather_parse.cpp
    weather_parse.hpp
    main.cpp
    ../common/utils.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(${PROJECT_NAME} PRIVATE
    ${VCPKG_ROOT}/include
    ../build/mqtt
)
target_link_directories(${PROJECT_NAME} PRIVATE
    ${VCPKG_ROOT}/lib
)
target_link_libraries(${PROJECT_NAME}
    ${EXTRA_LIBRARIES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
