#pragma once
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>

namespace xxlog {

enum class LogLevel { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };

class Logger {
public:
  static Logger &instance();

  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  void log(LogLevel level, const char *file, int line, const std::string &msg);

  void set_level(LogLevel level);
  void set_log_file(const std::string &filepath);

private:
  Logger();
  ~Logger();

  std::string get_time();

  static constexpr std::string_view level_to_string(LogLevel level) noexcept;

  LogLevel m_current_level;
  std::mutex mtx;
  std::unique_ptr<std::ofstream> m_file_stream;
};

} // namespace xxlog

#define XXLOG(level, msg)                                                      \
  xxlog::Logger::instance().log(level, __FILE__, __LINE__, msg)