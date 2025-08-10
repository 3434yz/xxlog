#include "logger.h"

int main() {
  xxlog::Logger::instance().setLevel(xxlog::LogLevel::TRACE);
  xxlog::Logger::instance().setLogFile("app.log");

  XXLOG(xxlog::LogLevel::INFO, "Hello from xxlog!");
  XXLOG(xxlog::LogLevel::ERROR, "Something went wrong!");

  return 0;
}
