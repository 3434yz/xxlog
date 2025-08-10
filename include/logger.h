#pragma once
#include <fstream>
#include <memory>
#include <mutex>
#include <string>

namespace xxlog {

enum class LogLevel { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };

class Logger {
public:
  static Logger &instance();
  void setLevel(LogLevel level);
  void setLogFile(const std::string &filepath);
  void log(LogLevel level, const char *file, int line, const std::string &msg);

private:
  Logger();
  ~Logger();

  std::string getTime();
  std::string levelToString(LogLevel level);

  LogLevel currentLevel;
  std::ofstream fileStream;
  std::mutex mtx;
};

} // namespace xxlog

#define XXLOG(level, msg)                                                      \
  xxlog::Logger::instance().log(level, __FILE__, __LINE__, msg)