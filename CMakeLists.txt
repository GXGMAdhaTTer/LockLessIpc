cmake_minimum_required(VERSION 3.17)

project(ipc-v2 CXX)

set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if($ENV{CROSS_COMPILING_TYPE} STREQUAL "arm")
    message("compile px2 platform")
    include(ArchConfiguration)
    include(CommonConfiguration)
    set(LIB_PLATFORM "aarch64-linux")
elseif($ENV{CROSS_COMPILING_TYPE} STREQUAL "arm_linux")
    message("compile arm linux platform")
    add_definitions("-DARM_PLATFORM")
    include(CommonConfiguration)
    set(LIB_PLATFORM "aarch64-linux")
elseif($ENV{CROSS_COMPILING_TYPE} STREQUAL "qnx700")
    message("compile qnx platform")
    add_definitions("-D_QNX_SOURCE")
    include_directories(${CMAKE_FIND_ROOT_PATH})
    set(LIB_PLATFORM "aarch64-qnx/7.0")
elseif($ENV{CROSS_COMPILING_TYPE} STREQUAL "qnx710")
    message("compile qnx platform")
    add_definitions("-DQNX_PLATFORM")
    add_definitions("-D_QNX_SOURCE")
    include_directories(${CMAKE_FIND_ROOT_PATH})
    set(LIB_PLATFORM "aarch64-qnx/7.1")
elseif($ENV{CROSS_COMPILING_TYPE} STREQUAL "x86")
    message("compile x86 platform")
    # add_definitions("-DEPOLL")
    # add_definitions("-DTIMERFD")
    # add_definitions("-DSHMGET")
    set(LIB_PLATFORM "x86_64-linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -march=native")
elseif($ENV{CROSS_COMPILING_TYPE} STREQUAL "a72_linux")
    message("compile A72 LINUX platform")
    set(LIB_PLATFORM "A72/aarch64-linux")
elseif($ENV{CROSS_COMPILING_TYPE} STREQUAL "a72_qnx")
    message("compile A72 QNX platform")
    add_definitions("-D_QNX_SOURCE")
    set(LIB_PLATFORM "A72/aarch64-qnx")
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/build/lib/${LIB_PLATFORM}/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build/bin/${LIB_PLATFORM}/${CMAKE_BUILD_TYPE})
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_CURRENT_LIST_DIR}/build/include)
set(CMAKE_SKIP_BUILD_RPATH TRUE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${LOCAL_W_FLAGS} ${LOCAL_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LOCAL_W_FLAGS} ${LOCAL_C_FLAGS} -std=c++17 -Wall -Wextra -fPIC")
set(CMAKE_SHARED_LINKER_FLAGS "-Xlinker --unresolved-symbols=report-all")

set(SOCK_SRCS
    ${CMAKE_SOURCE_DIR}/ipc.cpp
    ${CMAKE_SOURCE_DIR}/ipc_impl.cpp

    ${CMAKE_SOURCE_DIR}/common/buffer.cpp
    ${CMAKE_SOURCE_DIR}/common/spinlock.cpp
    ${CMAKE_SOURCE_DIR}/common/posix_timer.cpp

    ${CMAKE_SOURCE_DIR}/memorypool/memory_pool.cpp

    ${CMAKE_SOURCE_DIR}/socket/ppoll.cpp
    ${CMAKE_SOURCE_DIR}/socket/socket.cpp
    ${CMAKE_SOURCE_DIR}/socket/publisher.cpp
    ${CMAKE_SOURCE_DIR}/socket/subscriber.cpp
    ${CMAKE_SOURCE_DIR}/socket/broadcaster.cpp

    ${CMAKE_SOURCE_DIR}/shm/message_queue.cpp
    ${CMAKE_SOURCE_DIR}/shm/share_memory.cpp
)

if($ENV{CROSS_COMPILING_TYPE} STREQUAL "x86" OR $ENV{CROSS_COMPILING_TYPE} STREQUAL "arm" OR $ENV{CROSS_COMPILING_TYPE} STREQUAL "arm_linux")
    set(SOCK_SRCS ${SOCK_SRCS} common/timerfd.cpp socket/epoll.cpp)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/shm
    ${CMAKE_SOURCE_DIR}/socket
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/logger
    ${CMAKE_SOURCE_DIR}/xxhash
    ${CMAKE_SOURCE_DIR}/threadpool
    ${CMAKE_SOURCE_DIR}/memorypool
    ${CMAKE_SOURCE_DIR}/ratelimiter
    ${CMAKE_SOURCE_DIR}/nullmax_lib/nmlog/include
    ${CMAKE_INSTALL_INCLUDEDIR}
)

link_directories(
    ${LIBRARY_OUTPUT_PATH}
    ${CMAKE_SOURCE_DIR}/nullmax_lib/nmlog/lib/${LIB_PLATFORM}
)

add_library(ipc SHARED ${SOCK_SRCS})
if($ENV{CROSS_COMPILING_TYPE} STREQUAL "x86" OR $ENV{CROSS_COMPILING_TYPE} STREQUAL "arm" OR $ENV{CROSS_COMPILING_TYPE} STREQUAL "arm_linux")
    target_link_libraries(ipc ${PROJECT_LIBS} nmlog rt pthread)
else()
    target_link_libraries(ipc ${PROJECT_LIBS} nmlog c socket)
endif()

file(GLOB INSTALL_HEAD_FILES
    ${CMAKE_SOURCE_DIR}/ipc.h
    ${CMAKE_SOURCE_DIR}/ipc_config.h
)

install(FILES ${INSTALL_HEAD_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(test)
