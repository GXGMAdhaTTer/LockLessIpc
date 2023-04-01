#ifndef __LOG_IMPL_SYSBIOS_H__
#define __LOG_IMPL_SYSBIOS_H__

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <string.h>

namespace nullmax {
namespace log {

enum LogLevel { TRACE = 0, DEBUG, INFO, WARN, ERROR, FATAL, OFF };

const std::string LogLevelTag[] = {"T", "D", "I", "W", "E", "F"};

template <typename OStream, typename Arg>
void format_log(OStream &os, char *fmt, const Arg &arg) {
  char *begin = fmt;
  char *end = fmt + strlen(fmt) + 1;
  char *p = std::find(begin, end, '{');
  if (p == end) {
    std::cerr << "Invalid format string";
    return;
  }
  *p = '\0';
  os << begin;
  os << arg;
  begin = p + 1 + 1; // skip '}'
  if (begin < end) {
    os << begin;
  }
}

template <typename OStream>
void format_log(OStream &os, char *fmt) {
  os << fmt;
  return;
}

template <typename OStream, typename Arg1, typename... Args>
void format_log(OStream &os, char *fmt, const Arg1 &arg, const Args &... args) {
  char *begin = fmt;
  char *end = fmt + strlen(fmt) + 1;
  char *p = std::find(begin, end, '{');
  if (p == end) {
    std::cerr << "Invalid format string";
    return;
  }
  *p = '\0';
  os << begin;
  os << arg;
  begin = p + 1 + 1; // skip '}'
  format_log(os, begin, args...);
}

template <typename OStream>
class Logger {
 public:
  Logger<OStream>(OStream &os) : os_(os) {}

  void set_level(LogLevel level) { log_level_ = level; }

  template <typename... Args>
  void log(LogLevel level, const char *fmt, const Args &... args) {
    if (level < log_level_) {
      return;
    }
    uint32_t fmt_len = strlen(fmt) + 1;
    uint32_t ph_count = std::count(fmt, fmt + fmt_len, '{');
    uint32_t args_count = sizeof...(args);
    if (ph_count != args_count) {
      std::cerr << "Invalid parameters, number of placeholder characters "
                   "\"{}\" and arguments not match."
                << std::endl;
      return;
    }
    os_ << "[" << LogLevelTag[level] << " ";
    std::vector<char> fmt_vec(fmt, fmt + fmt_len);
    format_log(os_, fmt_vec.data(), args...);
    os_ << std::endl;
  }

  template <typename T>
  void log(LogLevel level, const T &msg) {
    if (level < log_level_) {
      return;
    }
    os_ << "[" << LogLevelTag[level] << " " << msg << std::endl;
  }

  template <typename... Args>
  inline void trace(const char *fmt, const Args &... args) {
    log(TRACE, fmt, args...);
  }

  template <typename... Args>
  inline void debug(const char *fmt, const Args &... args) {
    log(DEBUG, fmt, args...);
  }

  template <typename... Args>
  inline void info(const char *fmt, const Args &... args) {
    log(INFO, fmt, args...);
  }

  template <typename... Args>
  inline void warn(const char *fmt, const Args &... args) {
    log(WARN, fmt, args...);
  }

  template <typename... Args>
  inline void error(const char *fmt, const Args &... args) {
    log(ERROR, fmt, args...);
  }

  template <typename... Args>
  inline void critical(const char *fmt, const Args &... args) {
    log(FATAL, fmt, args...);
  }

  template <typename T>
  inline void trace(const T &msg) {
    log(TRACE, msg);
  }

  template <typename T>
  inline void debug(const T &msg) {
    log(DEBUG, msg);
  }

  template <typename T>
  inline void info(const T &msg) {
    log(INFO, msg);
  }

  template <typename T>
  inline void warn(const T &msg) {
    log(WARN, msg);
  }

  template <typename T>
  inline void error(const T &msg) {
    log(ERROR, msg);
  }

  template <typename T>
  inline void critical(const T &msg) {
    log(FATAL, msg);
  }

  void reset() { log_level_ = TRACE; }

  void flush() { os_.flush(); }

 private:
  LogLevel log_level_ = TRACE;
  OStream &os_;
};

typedef nullmax::log::Logger<std::ostream> OStreamLogger;

}  // namespace log
}  // namespace nullmax


#endif // __LOG_IMPL_SYSBIOS_H__