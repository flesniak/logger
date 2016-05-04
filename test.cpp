#include <iostream>
#include <string>

#include "logger.h"

const std::string logFile = "test.log";

int main(int argc, char** argv) {
  LoggerFacilityConsole lfc;
  LoggerFacilityFile lff;
  if (!lff.openLogFile(logFile))
    std::cerr << "failed to open log file " << logFile << std::endl;
  Logger::registerFacility(&lff);
  Logger::registerFacility(&lfc);
  Logger::logLevel() = logDebug;
  Logger::logLevel() = (logLevel_t)5;

  LOG((logLevel_t)5) << "debug+1 message";
  LOG(logDebug) << "debug message";
  LOG(logDetail) << "detailed message";
  LOG(logInfo) << "info message";
  LOG(logWarning) << "warning message";
  LOG(logError) << "error message";

  LOG(logDebug) << "argc " << argc << " argv " << argv;

  LOG(logDebug) << "removing file facility";
  Logger::unregisterFacility(&lff);
  LOG(logDebug) << "removing console facility";
  Logger::unregisterFacility(&lfc);
  std::cerr << "removing file facility again " << Logger::unregisterFacility(&lff) << std::endl;
  return 0;
}
