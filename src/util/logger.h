#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>

using namespace std;

namespace util {
    
/**
* @brief Enums for defining the verbosity level of the log message.
**/
enum LogLevel {logFATAL, logERROR, logWARNING, logINFO, logDEBUG};

/**
 * @brief Simple logging class. Best used with the LOG(LogLevel) macro.
 *
 * Example:
 * @code
 * #include "logger.h"
 * LOG(logINFO) << "Log" << 1 << ": Test.";
 * @endcode
 * 
 **/
class Logger
{
public:
    /**
     * @brief Stream log message to stderr and flush.
     * 
     **/
    ~Logger();
    
    /**
     * @brief Log message to ostream.
     *
     * Use of macro LOG(LogLevel) recommended.
     *
     * @param level Verbosity level. Defaults to logINFO.
     * @return The log ostream.
     **/
    ostringstream& log(LogLevel level = logINFO);
    
    /**
     * @return Current verbosity level.
     **/
    static LogLevel getVerbosityLevel();
private:
    std::ostringstream ostream;
    
    static const LogLevel currentVerbosityLevel = logWARNING;
};

}

#define LOG(level) \
if (level > Logger::getVerbosityLevel()) ; \
else Logger().log(level) << __FUNCTION__ << ": "

#endif // LOGGER_H
