#include "logger.h"
#include <iostream>

void Logger::setLevel(LogLevel level) {
  // 后续实现
}

void Logger::log(LogLevel level, std::string_view msg) {
  std::cout << "[LOG] " << msg << std::endl;
}
