#pragma once
#include <string_view>

enum class LogLevel { Debug, Info, Warn, Error };

class Logger {
public:
  void setLevel(LogLevel level);

  template <typename... Args> void info(std::string_view msg, Args &&...args) {
    log(LogLevel::Info, msg, std::forward<Args>(args)...);
  }

  template <typename... Args> void error(std::string_view msg, Args &&...args) {
    log(LogLevel::Error, msg, std::forward<Args>(args)...);
  }

private:
  void log(LogLevel level, std::string_view msg);
};

class LoggerFactory {}
