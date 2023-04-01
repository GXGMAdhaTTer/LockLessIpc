#ifndef __LOG_H__
#define __LOG_H__

#include "log_impl.h"

#include <stdio.h>
#include <chrono>
#include <sstream>
#include <string>

#define __FILE_NAME__ \
  (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)

#ifdef __cplusplus
#define __LOG_SUFFIX__(fmt)             \
  std::string(__FILE_NAME__)            \
      .append(":")                      \
      .append(std::to_string(__LINE__)) \
      .append("] ")                     \
      .append(fmt)                      \
      .c_str()

#define __STREAM_SUFFIX__()             \
  std::string(__FILE_NAME__)            \
      .append(":")                      \
      .append(std::to_string(__LINE__)) \
      .append("] ")

#else
#define __LOG_SUFFIX__(fmt) fmt
#define __STREAM_SUFFIX__()
#endif

#define g_logger nullmax::nmlog::NullmaxLog::LogInstance()

#ifdef DISABLE_DEBUG_LOG
#define NM_DEBUG(...)
#define NM_INFO(...)
#else
#define NM_DEBUG(fmt, ...) g_logger->debug(__LOG_SUFFIX__(fmt), ##__VA_ARGS__)
#define NM_INFO(fmt, ...) g_logger->info(__LOG_SUFFIX__(fmt), ##__VA_ARGS__)
#endif
#define NM_WARN(fmt, ...)                                    \
  {                                                          \
    g_logger->warn(__LOG_SUFFIX__(fmt), ##__VA_ARGS__);      \
  }
#define NM_ERROR(fmt, ...)                                   \
  {                                                          \
    g_logger->error(__LOG_SUFFIX__(fmt), ##__VA_ARGS__);     \
  }

#define NM_NO_SUFFIX_DEBUG(fmt, ...) g_logger->debug(fmt, ##__VA_ARGS__)
#define NM_NO_SUFFIX_INFO(fmt, ...) g_logger->info(fmt, ##__VA_ARGS__)
#define NM_NO_SUFFIX_WARN(fmt, ...) g_logger->warn(fmt, ##__VA_ARGS__)
#define NM_NO_SUFFIX_ERROR(fmt, ...) g_logger->error(fmt, ##__VA_ARGS__)

// glog style logging, like LOG(INFO) << xx << xxx;
#define LOG(LEVEL)                                                           \
  if (nullmax::nmlog::LEVEL >=                                               \
      nullmax::nmlog::NullmaxLog::LogInstance()->level())                    \
  nullmax::nmlog::LogMessage(__FILE_NAME__, __LINE__, nullmax::nmlog::LEVEL) \
      .stream()

#define NULLMAX_LOG_SET_LEVEL(x) \
  nullmax::nmlog::NullmaxLog::Instance()->set_level(nullmax::nmlog::x);

#define NULLMAX_LOG_SET_DIR(x) \
  nullmax::nmlog::NullmaxLog::Instance()->set_log_dir(x);

// legacy initialization
#define INIT_NULLMAX_LOG(name, log_dir, level)                \
  {                                                           \
    nullmax::nmlog::InitNullmaxLog(name, level, log_dir);     \
  }

#define DROP_ALL_NULLMAX_LOG()                         \
  {                                                    \
    nullmax::nmlog::DropAllNullmaxLog();               \
  }

// NM_LOG_IF
#define NM_DEBUG_IF(flag, ...)       \
  {                                  \
    if (flag) NM_DEBUG(__VA_ARGS__); \
  }
#define NM_INFO_IF(flag, ...)       \
  {                                 \
    if (flag) NM_INFO(__VA_ARGS__); \
  }
#define NM_WARN_IF(flag, ...)       \
  {                                 \
    if (flag) NM_WARN(__VA_ARGS__); \
  }
#define NM_ERROR_IF(flag, ...)       \
  {                                  \
    if (flag) NM_ERROR(__VA_ARGS__); \
  }

// NM_LOG_ONCE
#define NM_DEBUG_ONCE(...)   \
  {                          \
    static bool flag = true; \
    if (flag) {              \
      NM_DEBUG(__VA_ARGS__); \
      flag = false;          \
    }                        \
  }
#define NM_INFO_ONCE(...)    \
  {                          \
    static bool flag = true; \
    if (flag) {              \
      NM_INFO(__VA_ARGS__);  \
      flag = false;          \
    }                        \
  }

#define NM_WARN_ONCE(...)    \
  {                          \
    static bool flag = true; \
    if (flag) {              \
      NM_WARN(__VA_ARGS__);  \
      flag = false;          \
    }                        \
  }
#define NM_ERROR_ONCE(...)   \
  {                          \
    static bool flag = true; \
    if (flag) {              \
      NM_ERROR(__VA_ARGS__); \
      flag = false;          \
    }                        \
  }

// NM_LOG_STREAM
#define NM_DEBUG_STREAM(...)                  \
  {                                           \
    std::ostringstream ss;                    \
    ss << __STREAM_SUFFIX__() << __VA_ARGS__; \
    NM_DEBUG(ss.str());                       \
  }
#define NM_INFO_STREAM(...)                   \
  {                                           \
    std::ostringstream ss;                    \
    ss << __STREAM_SUFFIX__() << __VA_ARGS__; \
    NM_INFO(ss.str());                        \
  }
#define NM_WARN_STREAM(...)                   \
  {                                           \
    std::ostringstream ss;                    \
    ss << __STREAM_SUFFIX__() << __VA_ARGS__; \
    NM_WARN(ss.str());                        \
  }
#define NM_ERROR_STREAM(...)                  \
  {                                           \
    std::ostringstream ss;                    \
    ss << __STREAM_SUFFIX__() << __VA_ARGS__; \
    NM_ERROR(ss.str());                       \
  }

#define NM_DEBUG_THROTTLE(DURATION, ...)                                    \
  {                                                                         \
    static auto start = std::chrono::system_clock::now();                   \
    auto end = std::chrono::system_clock::now();                            \
    auto duration =                                                         \
        std::chrono::duration_cast<std::chrono::microseconds>(end - start); \
    auto duration_time = double(duration.count()) *                         \
                         std::chrono::microseconds::period::num /           \
                         std::chrono::microseconds::period::den;            \
    if (duration_time > DURATION) {                                         \
      NM_DEBUG(__VA_ARGS__);                                                \
      start = end;                                                          \
    }                                                                       \
  }

#define NM_INFO_THROTTLE(DURATION, ...)                                     \
  {                                                                         \
    static auto start = std::chrono::system_clock::now();                   \
    auto end = std::chrono::system_clock::now();                            \
    auto duration =                                                         \
        std::chrono::duration_cast<std::chrono::microseconds>(end - start); \
    auto duration_time = double(duration.count()) *                         \
                         std::chrono::microseconds::period::num /           \
                         std::chrono::microseconds::period::den;            \
    if (duration_time > DURATION) {                                         \
      NM_INFO(__VA_ARGS__);                                                 \
      start = end;                                                          \
    }                                                                       \
  }

#define NM_WARN_THROTTLE(DURATION, ...)                                     \
  {                                                                         \
    static auto start = std::chrono::system_clock::now();                   \
    auto end = std::chrono::system_clock::now();                            \
    auto duration =                                                         \
        std::chrono::duration_cast<std::chrono::microseconds>(end - start); \
    auto duration_time = double(duration.count()) *                         \
                         std::chrono::microseconds::period::num /           \
                         std::chrono::microseconds::period::den;            \
    if (duration_time > DURATION) {                                         \
      NM_WARN(__VA_ARGS__);                                                 \
      start = end;                                                          \
    }                                                                       \
  }

#define NM_ERROR_THROTTLE(DURATION, ...)                                    \
  {                                                                         \
    static auto start = std::chrono::system_clock::now();                   \
    auto end = std::chrono::system_clock::now();                            \
    auto duration =                                                         \
        std::chrono::duration_cast<std::chrono::microseconds>(end - start); \
    auto duration_time = double(duration.count()) *                         \
                         std::chrono::microseconds::period::num /           \
                         std::chrono::microseconds::period::den;            \
    if (duration_time > DURATION) {                                         \
      NM_ERROR(__VA_ARGS__);                                                \
      start = end;                                                          \
    }                                                                       \
  }


#endif // __LOG_H__