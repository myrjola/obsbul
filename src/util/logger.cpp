#include "logger.h"

#include <iostream>

namespace util {
    
Logger::~Logger()
{
   ostream << endl;
   cerr << ostream.str();
   cerr.flush();
}

LogLevel Logger::getVerbosityLevel()
{
    return currentVerbosityLevel;
}

std::ostringstream& Logger::log(LogLevel level)
{
    return ostream;
}
 
}


