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

Logger::Logger() : m_current_level(LogLevel::DEBUG), m_file_stream(nullptr) {}

Logger::~Logger() {
  std::lock_guard lock(mtx);
  if (m_file_stream && m_file_stream->is_open()) {
    m_file_stream->flush();
    m_file_stream->close();
  }
}

void Logger::set_level(LogLevel level) { m_current_level = level; }

  void Logger::set_log_file(const std::string &filepath) {
  std::lock_guard lock(mtx);  // 确保线程安全

  // 如果文件流未初始化，直接创建并打开（默认模式）
  if (!m_file_stream) {
    m_file_stream = std::make_unique<std::ofstream>(filepath, std::ios::app);
  } else {
    // 若已打开，先关闭
    if (m_file_stream->is_open()) {
      m_file_stream->close();
    }
    // 重新打开指定文件（追加模式）
    m_file_stream->open(filepath, std::ios::app);
  }

  // 检查文件是否成功打开
  if (!m_file_stream->is_open()) {
    std::cerr << "Failed to open log file: " << filepath << std::endl;
    // 可选：清空智能指针，标记无效状态
    m_file_stream.reset();
  }
}


void Logger::log(LogLevel level, const char *file, int line,
                 const std::string &msg) {
  if (level < m_current_level)
    return;

  std::string timeStr = get_time();
  std::string_view levelStr = level_to_string(level);

  std::ostringstream oss;
  oss << "[" << timeStr << "][" << levelStr << "]";
  if (file) {
    oss << "[" << file << ":" << line << "]";
  }
  oss << " " << msg << "\n";

  std::string logLine = oss.str();

  // 确保线程安全
  std::lock_guard<std::mutex> lock(mtx);

  // 输出到控制台
  std::cout << logLine;
  std::cout.flush();

  // 如果设置了文件输出
  if (m_file_stream && m_file_stream->is_open()) {
    (*m_file_stream) << logLine;
    m_file_stream->flush();
  }
}

std::string Logger::get_time() {
  using namespace std::chrono;
  const auto now = system_clock::now();
  auto itt = system_clock::to_time_t(now);
  const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

  std::ostringstream ss;
#if defined(_MSC_VER)
  // msvc: localtime_s
  std::tm t;
  localtime_s(&t, &itt);
  ss << std::put_time(&t, "%Y-%m-%d %H:%M:%S");
#else
  const std::tm *t = std::localtime(&itt);
  ss << std::put_time(t, "%Y-%m-%d %H:%M:%S");
#endif
  ss << "." << std::setfill('0') << std::setw(3) << ms.count();
  return ss.str();
}

constexpr std::string_view Logger::level_to_string(const LogLevel level) noexcept {
  using namespace std::literals;
  switch (level) {
  case LogLevel::TRACE:
    return "TRACE"sv;
  case LogLevel::DEBUG:
    return "DEBUG"sv;
  case LogLevel::INFO:
    return "INFO"sv;
  case LogLevel::WARN:
    return "WARN"sv;
  case LogLevel::ERROR:
    return "ERROR"sv;
  case LogLevel::FATAL:
    return "FATAL"sv;
  default:
    return "UNKNOWN"sv;
  }
}
} // namespace xxlog
