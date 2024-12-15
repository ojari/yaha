set(TOOLCHAIN_PREFIX aarch64-linux-gnu)

set(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)

set(VCPKG_DIR ${CMAKE_SOURCE_DIR}/vcpkg_installed)
set(VCPKG_ROOT ${VCPKG_DIR}/arm64-linux-release)
set(VCPKG_TOOLS ${VCPKG_DIR}/x64-linux/tools)
