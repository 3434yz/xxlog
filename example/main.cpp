#include "logger.h"

int main() {
  xxlog::Logger::instance().set_level(xxlog::LogLevel::TRACE);
  xxlog::Logger::instance().set_log_file("app.log");

  XXLOG(xxlog::LogLevel::INFO, "Hello from xxlog!");
  XXLOG(xxlog::LogLevel::ERROR, "Something went wrong!");

  return 0;
}
