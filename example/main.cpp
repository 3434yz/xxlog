#include "logger.h"

int main() {
  Logger logger;
  logger.info("hello xxlog");
  logger.error("something wrong");

  return 0;
}
