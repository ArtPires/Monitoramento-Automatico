#ifndef _LOG_H_
#define _LOG_H_

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "Commons.h"

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_WHITE   "\033[37m"

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Log {
public:

    static void setLogFile(std::string fileName);
    static void setLogLevel(LogLevel level);
    static void log(LogLevel level, const std::string& message);

    static void debug(const std::string& msg);
    static void info(const std::string& msg);
    static void warning(const std::string& msg);
    static void error(const std::string& msg);

private:
    static LogLevel configLogLevel;
    static std::ofstream logFile;

    static std::string getTimestamp();
    static std::string levelToString(LogLevel level);
    
};

#endif //_LOG_H_