SET(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

SET(PDKIPU04 $ENV{PDKIPU04})
SET(CROSSBIN ${PDKIPU04}/toolchains/aarch64--glibc--stable-2020.08-1/bin/aarch64-buildroot-linux-gnu-)

set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".so")

SET(CMAKE_C_COMPILER ${CROSSBIN}gcc)
SET(CMAKE_CXX_COMPILER ${CROSSBIN}g++)

set(CUSTOM_COMPILE_OPTIONS -fPIC -Wall -fms-extensions -Wno-write-strings -Wno-format-security -fno-short-enums)
if(TARGET_BUILD STREQUAL "debug")
    list(APPEND CUSTOM_COMPILE_OPTIONS -ggdb -ggdb3 -gdwarf-2 -D_DEBUG_=1)
else()
    list(APPEND CUSTOM_COMPILE_OPTIONS -O3 -DNDEBUG)
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CUSTOM_COMPILE_OPTIONS}")
string(REPLACE ";" " " CMAKE_C_FLAGS "${CUSTOM_COMPILE_OPTIONS}")
