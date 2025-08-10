#include "logger.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace xxlog {
Logger &Logger::instance() {
  static Logger inst;
  return inst;
}

Logger::Logger() : currentLevel(LogLevel::INFO) {}

Logger::~Logger() {
  if (fileStream.is_open()) {
    fileStream.close();
  }
}

void Logger::setLevel(LogLevel level) { currentLevel = level; }

void Logger::setLogFile(const std::string &filepath) {
  std::lock_guard<std::mutex> lock(mtx);
  if (fileStream.is_open()) {
    fileStream.close();
  }
  fileStream.open(filepath, std::ios::app);
}

void Logger::log(LogLevel level, const char *file, int line,
                 const std::string &msg) {
  if (level < currentLevel)
    return;

  std::lock_guard<std::mutex> lock(mtx);
  std::string timeStr = getTime();
  std::string levelStr = levelToString(level);

  std::ostringstream oss;
  oss << "[" << timeStr << "][" << levelStr << "][" << file << ":" << line
      << "] " << msg << "\n";

  std::string logLine = oss.str();

  // 输出到控制台
  std::cout << logLine;
  std::cout.flush();

  // 输出到文件
  if (fileStream.is_open()) {
    fileStream << logLine;
    fileStream.flush();
  }
}

std::string Logger::getTime() {
  using namespace std::chrono;
  auto now = system_clock::now();
  auto itt = system_clock::to_time_t(now);
  auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

  std::ostringstream ss;
  ss << std::put_time(std::localtime(&itt), "%Y-%m-%d %H:%M:%S") << "."
     << std::setfill('0') << std::setw(3) << ms.count();
  return ss.str();
}

std::string Logger::levelToString(LogLevel level) {
  switch (level) {
  case LogLevel::TRACE:
    return "TRACE";
  case LogLevel::DEBUG:
    return "DEBUG";
  case LogLevel::INFO:
    return "INFO";
  case LogLevel::WARN:
    return "WARN";
  case LogLevel::ERROR:
    return "ERROR";
  case LogLevel::FATAL:
    return "FATAL";
  default:
    return "UNKNOWN";
  }
}
} // namespace xxlog
