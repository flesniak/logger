#include "logger.h"

#include <ctime>
#include <algorithm>
#include <unordered_map>
#include <string>

Logger::Logger()
{
}

std::ostringstream& Logger::getLogger(logLevel_t level)
{
  os << getTime();
  os << " " << toString(level) << ": ";
  //os << std::string(level > logDebug ? level - logDebug : 0, '\t');
  return os;
}

Logger::~Logger()
{
  os << std::endl;
  //for_each(facilities().begin(), facilities().end(), [this](LoggerFacility* lf){lf->output(os.str());});
  for(auto f : facilities())
    f->output(os.str());
}

void Logger::registerFacility(LoggerFacility* lf)
{
  Logger::facilities().push_back(lf);
}

bool Logger::unregisterFacility(LoggerFacility* lf)
{
  auto it = find(facilities().begin(), facilities().end(), lf);
  if (it != facilities().end()) {
    facilities().erase(it);
    return true;
  }
  return false;
}

std::vector<LoggerFacility*>& Logger::facilities()
{
  static std::vector<LoggerFacility*> lfs;
  return lfs;
}

logLevel_t Logger::fromString(const std::string& level)
{
    if (level == "DEBUG")
        return logDebug;
    if (level == "DETAIL")
        return logDetail;
    if (level == "INFO")
        return logInfo;
    if (level == "WARNING")
        return logWarning;
    if (level == "ERROR")
        return logError;
    Logger().getLogger(logWarning) << "Unknown logging level '" << level << "'. Using INFO level as default.";
    return logInfo;
}

inline std::string Logger::getTime()
{
  char buffer[11];
  time_t t = time(0);
  strftime(buffer, sizeof(buffer), "%X", localtime(&t));
  return buffer;
}

logLevel_t& Logger::logLevel()
{
  static logLevel_t reportingLevel = logDebug;
  return reportingLevel;
}

std::string Logger::toString(logLevel_t level)
{
  const std::unordered_map<int, std::string> strmap = {
    {logError, "ERROR"},
    {logWarning, "WARNING"},
    {logInfo, "INFO"},
    {logDetail, "DETAIL"},
    {logDebug, "DEBUG"}};
  std::string str;
  try {
    str = strmap.at(level);
  } catch(std::out_of_range e) {
    str = "CUSTOM"+std::to_string(level-logLevelCount+1);
  }
  return str;
  //static const char* const buffer[] = { "ERROR", "WARNING", "INFO", "DETAIL", "DEBUG" };
  //return buffer[level];
}

LoggerFacility::~LoggerFacility()
{
}

LoggerFacilityConsole::~LoggerFacilityConsole()
{
}

inline std::ostream& LoggerFacilityConsole::stream()
{
  static std::ostream* pStream = &std::cerr;
  return *pStream;
}

inline void LoggerFacilityConsole::output(const std::string& msg)
{
  stream() << msg;
}

LoggerFacilityFile::~LoggerFacilityFile()
{
  if( p_fs.is_open() )
    p_fs.close();
}

void LoggerFacilityFile::closeLogFile()
{
  if( p_fs.is_open() )
    p_fs.close();
}

bool LoggerFacilityFile::openLogFile(const std::string& path, bool truncate)
{
  closeLogFile();
  if( truncate )
    p_fs.open(path.c_str(), std::ios_base::out | std::ios_base::trunc);
  else
    p_fs.open(path.c_str(), std::ios_base::out | std::ios_base::app);
  return p_fs.is_open();
}

void LoggerFacilityFile::output(const std::string& msg)
{
  p_fs << msg;
  p_fs.flush();
}
