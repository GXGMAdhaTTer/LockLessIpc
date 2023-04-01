#ifndef __LOG_IMPL_H__
#define __LOG_IMPL_H__

#ifdef SYSBIOS
#include "log_impl_sysbios.h"
#else
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"
#endif

#include <stdio.h>
#ifndef C66
#include <atomic>
#endif
#include <memory>
#include <sstream>
#include <stack>
#include <string>

namespace nullmax {
namespace nmlog {

#define MAX_LOG_FILE_SIZE 200 * 1024 * 1024
#define MAX_FILES 1000

#ifdef R5F
extern std::atomic<bool> __init_flag;
#elif defined(C66)
extern bool __init_flag;
#else
extern std::once_flag __init_flag;
#endif

#ifdef SYSBIOS
enum LogLevel {
  TRACE = nullmax::log::TRACE,
  DEBUG = nullmax::log::DEBUG,
  INFO = nullmax::log::INFO,
  WARN = nullmax::log::WARN,
  ERROR = nullmax::log::ERROR,
  FATAL = nullmax::log::FATAL,
  OFF = nullmax::log::OFF
};
using LoggerPtr = std::shared_ptr<nullmax::log::OStreamLogger>;
#else
enum LogLevel {
  TRACE = spdlog::level::level_enum::trace,
  DEBUG = spdlog::level::level_enum::debug,
  INFO = spdlog::level::level_enum::info,
  WARN = spdlog::level::level_enum::warn,
  ERROR = spdlog::level::level_enum::err,
  FATAL = spdlog::level::level_enum::critical,
  OFF = spdlog::level::level_enum::off
};

using LoggerPtr = std::shared_ptr<spdlog::logger>;
#endif

// for program name
// http://www.opencoverage.net/coreutils/index_html/source_149.html

// match level_enum in spdlog/common.h
class NullmaxLog {
 public:
  NullmaxLog() {}
  ~NullmaxLog() {}

  void Init() {
    InitProgramName();
    InitLogDir(log_dir_.empty() ? "." : log_dir_);
    InitLoggers();
  }

  void Destroy() {
    logger_.reset();
    err_logger_.reset();
    warn_logger_.reset();
#ifndef SYSBIOS
    spdlog::drop_all();
#endif
  }

  // will not init logger instance, use this method to init manually
  static NullmaxLog *Instance() { return &instance_; }
  // will guarantee the logger instance is initialized properly
  static NullmaxLog *LogInstance() {
#ifdef R5F
    if (!__init_flag.exchange(true)) {
      instance_.Init();
    }
#elif defined(C66)
    if (!__init_flag) {
      instance_.Init();
    }
#else
    std::call_once(__init_flag, &NullmaxLog::Init, &instance_);
#endif
    return &instance_;
  }

  void set_program_name(const std::string &name) { program_name_ = name; }
  std::string program_name() const { return program_name_; }

  LoggerPtr logger() const { return logger_; }
  LoggerPtr err_logger() const { return err_logger_; }
  LoggerPtr warn_logger() const { return warn_logger_; }

  void set_level(LogLevel level) {
    level_ = level;
    if (logger_) {
#ifdef SYSBIOS
      logger_->set_level(static_cast<nullmax::log::LogLevel>(level));
#else
      spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
#endif
    }
  }
  LogLevel level() const { return level_; }

  void set_log_dir(const std::string &dir) {
    log_dir_ = dir.empty() ? "." : dir;
  }
  std::string log_dir() const { return log_dir_; }

  template <typename... Args>
  inline void trace(const char *fmt, const Args &... args) {
    logger_->trace(fmt, args...);
  }

  template <typename... Args>
  inline void debug(const char *fmt, const Args &... args) {
    logger_->debug(fmt, args...);
  }

  template <typename... Args>
  inline void info(const char *fmt, const Args &... args) {
    logger_->info(fmt, args...);
  }

  template <typename... Args>
  inline void warn(const char *fmt, const Args &... args) {
    logger_->warn(fmt, args...);
#ifndef SYSBIOS
    warn_logger_->warn(fmt, args...);
#endif
  }

  template <typename... Args>
  inline void error(const char *fmt, const Args &... args) {
    logger_->error(fmt, args...);
#ifndef SYSBIOS
    err_logger_->error(fmt, args...);
#endif
  }

  template <typename... Args>
  inline void critical(const char *fmt, const Args &... args) {
    logger_->critical(fmt, args...);
  }

 private:
  void InitProgramName() {
    if (!program_name_.empty()) {
      return;
    }
#if defined(_GNU_SOURCE)  // glibc
    program_name_ = program_invocation_short_name;
#else
    program_name_ = "nmlog_defualt";
#endif
  }
  void InitLoggers();
  void InitLogDir(const std::string &log_path);
  void InitLogDir(const std::stack<std::string> &path_stack);

  std::string program_name_;
  LoggerPtr logger_;
  LoggerPtr err_logger_;
  LoggerPtr warn_logger_;
  LogLevel level_ = INFO;

  std::string log_dir_ = "./";
  static constexpr const int32_t kMaxLogFileSize = MAX_LOG_FILE_SIZE;
  static constexpr const int32_t kMaxFiles = MAX_FILES;

  static NullmaxLog instance_;
};

class LogMessage {
 public:
  LogMessage(const char *file, int32_t line, LogLevel level)
      : file_(file), line_(line), level_(level) {
    stream_ << file << ":" << line << "] ";
  }

  ~LogMessage() {
    auto nmlog = NullmaxLog::LogInstance();
    switch (level_) {
      case DEBUG:
        nmlog->logger()->debug(stream_.str());
        break;
      case INFO:
        nmlog->logger()->info(stream_.str());
        break;
      case WARN:
        nmlog->logger()->warn(stream_.str());
#ifndef SYSBIOS
        nmlog->warn_logger()->warn(stream_.str());
#endif
        break;
      case ERROR:
        nmlog->logger()->error(stream_.str());
#ifndef SYSBIOS
        nmlog->err_logger()->error(stream_.str());
#endif
        break;
      case FATAL:
        nmlog->logger()->error(stream_.str());
#ifndef SYSBIOS
        nmlog->err_logger()->error(stream_.str());
#endif
        assert(false);
        break;
      default:
        break;
    }
  }

  LogMessage(const LogMessage &) = delete;
  void operator=(const LogMessage &) = delete;

  std::ostream &stream() { return stream_; }

 private:
  const char *file_;
  const int32_t line_;
  const LogLevel level_;

  std::ostringstream
      stream_;  // stringstream for less code but less performance
};

void InitNullmaxLog(const std::string &name, nullmax::nmlog::LogLevel level,
                    const std::string &log_dir);
void DropAllNullmaxLog();
}  // namespace nmlog
}  // namespace nullmax


#endif // __LOG_IMPL_H__