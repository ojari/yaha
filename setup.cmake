if (YOCTO) 
  set(VCPKG_ROOT /usr)
  set(VCPKG_TOOLS /usr)
  set(CURL_LIB curl)
  set(EXTRA_LIBRARIES ssl crypto pthread dl z)
  set(MOSQUITTO_LIB mosquitto)
else()
  if (WIN32)
    set(VCPKG_DIR c:/usr/vcpkg)
    set(VCPKG_ROOT ${VCPKG_DIR}/installed/x64-windows)
    set(VCPKG_TOOLS ${VCPKG_ROOT}/tools)
    set(CURL_LIB libcurl)
    set(EXTRA_LIBRARIES libcrypto libssl)
    set(MOSQUITTO_LIB mosquitto)
  else()
    #set(VCPKG_DIR /opt/vcpkg)
    #set(VCPKG_DIR /home/jari/vcpkg/installed)
    set(VCPKG_DIR ${CMAKE_SOURCE_DIR}/vcpkg_installed)
    set(VCPKG_ROOT ${VCPKG_DIR}/x64-linux-release)
    set(VCPKG_TOOLS ${VCPKG_DIR}/x64-linux/tools)
    set(CURL_LIB curl)
    set(EXTRA_LIBRARIES ssl crypto pthread dl z fmt)
    set(MOSQUITTO_LIB mosquitto_static)
  endif()
  include_directories(${VCPKG_ROOT}/include)
  link_directories(${VCPKG_ROOT}/lib)
  # set(CMAKE_TOOLCHAIN_FILE ${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake)
endif()
