set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(TOOLCHAIN "$ENV{PSDKRA_PATH}/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu")

set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".so")

set(CMAKE_C_COMPILER "${TOOLCHAIN}/bin/aarch64-none-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN}/bin/aarch64-none-linux-gnu-g++")

set(CUSTOM_COMPILE_OPTIONS -fPIC -Wall -fms-extensions -Wno-write-strings -Wno-format-security -fno-short-enums -mlittle-endian -mcpu=cortex-a72)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND CUSTOM_COMPILE_OPTIONS -ggdb -ggdb3 -gdwarf-2 -D_DEBUG_=1)
else()
    list(APPEND CUSTOM_COMPILE_OPTIONS -O3 -DNDEBUG)
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CUSTOM_COMPILE_OPTIONS}")
string(REPLACE ";" " " CMAKE_C_FLAGS "${CUSTOM_COMPILE_OPTIONS}")

include_directories(BEFORE SYSTEM
    $ENV{PSDKRA_PATH}/xdctools_3_61_00_16_core/packages
    $ENV{PSDKRA_PATH}/bios_6_82_01_19/packages
    $ENV{PSDKRA_PATH}/pdk_jacinto_07_00_00/packages
    $ENV{PSDKRA_PATH}/ndk_3_75_01_01/packages
    $ENV{PSDKRA_PATH}/vxlib_c66x_1_1_5_0/packages
    $ENV{PSDKRA_PATH}/tidl_j7_01_02_00_09/ti_dl/inc
    $ENV{PSDKRA_PATH}/ivision
    $ENV{PSDKRA_PATH}/imaging
    $ENV{PSDKRA_PATH}/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/aarch64-linux-gnu/libc/usr/include/
    $ENV{PSDKRA_PATH}/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/aarch64-linux-gnu/include/c++/8.3.0
    $ENV{PSDKRA_PATH}/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/aarch64-linux-gnu/include/c++/8.3.0/aarch64-linux-gnu
    $ENV{PSDKRA_PATH}/targetfs
    $ENV{PSDKRA_PATH}/targetfs/usr/include
)

link_directories(BEFORE 
    $ENV{PSDKRA_PATH}/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/aarch64-linux-gnu/libc/usr/lib/
    $ENV{PSDKRA_PATH}/targetfs/usr/lib
)

add_definitions(-DLINUX -DJ7 -DA72="A72" -DTARGET_NUM_CORES=1 -DTARGET_ARCH=64 -DARCH_64 -DARM)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DTARGET_BUILD=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DTARGET_BUILD=2)
else()
    add_definitions(-DTARGET_BUILD=1)
endif()