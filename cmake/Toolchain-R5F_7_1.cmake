set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(TOOLCHAIN "$ENV{PSDKRA_PATH}/ti-cgt-arm_20.2.0.LTS")

set(CMAKE_C_COMPILER "${TOOLCHAIN}/bin/armcl")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN}/bin/armcl")

set(CUSTOM_COMPILE_OPTIONS --endian=little --abi=eabi -mv=7R5 --float_support=vfpv3d16 --symdebug:none --rtti --no_warnings)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND CUSTOM_COMPILE_OPTIONS -g3 -ggdb -ggdb3 -D_DEBUG_=1)
else()
    list(APPEND CUSTOM_COMPILE_OPTIONS --opt_level=3 --gen_opt_info=2 -DNDEBUG)
endif()

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CUSTOM_COMPILE_OPTIONS}")
string(REPLACE ";" " " CMAKE_C_FLAGS "${CUSTOM_COMPILE_OPTIONS}")

include_directories(BEFORE SYSTEM
    $ENV{PSDKRA_PATH}/ti-cgt-arm_20.2.0.LTS/include
    $ENV{PSDKRA_PATH}/xdctools_3_61_03_29_core/packages
    $ENV{PSDKRA_PATH}/bios_6_83_00_18/packages
    $ENV{PSDKRA_PATH}/pdk_jacinto_07_01_00_45/packages
    $ENV{PSDKRA_PATH}/ndk_3_80_00_19/packages
    ${PROJECT_SOURCE_DIR}/nullmax_lib/ti/nullmax/nm_tirtos/
    $ENV{PSDKRA_PATH}/bios_6_83_00_18/packages/ti/posix/ccs
    ${PROJECT_SOURCE_DIR}/nullmax_lib/ti/nullmax/nm_tirtos/sys
    ${PROJECT_SOURCE_DIR}/nullmax_lib/ti/nullmax/nm_tirtos/bits
)

link_directories(BEFORE
    $ENV{PSDKRA_PATH}/ti-cgt-arm_20.2.0.LTS/lib
)

add_definitions(-DSYSBIOS -DJ7 -DR5F="R5F" -DTARGET_NUM_CORES=3 -DTARGET_ARCH=32 -DARCH_32 -DARM -Dxdc_target_name__=ti/targets/arm/elf/R5F -Dxdc_target_types__=ti/targets/arm/elf/std.h)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DTARGET_BUILD=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DTARGET_BUILD=2)
else()
    add_definitions(-DTARGET_BUILD=1)
endif()
