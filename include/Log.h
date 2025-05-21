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
    
    template<typename T>
    static void debug(const T& msg) {
        log(LogLevel::DEBUG, toString(msg));
    }

    template<typename T>
    static void info(const T& msg) {
        log(LogLevel::INFO, toString(msg));
    }

    template<typename T>
    static void warning(const T& msg) {
        log(LogLevel::WARNING, toString(msg));
    }

    template<typename T>
    static void error(const T& msg) {
        log(LogLevel::ERROR, toString(msg));
    }
    
    private:
    static LogLevel configLogLevel;
    static std::ofstream logFile;
    
    static std::string getTimestamp();
    static std::string levelToString(LogLevel level);
    static void log(LogLevel level, const std::string& message);
    
    template<typename T>
    static std::string toString(const T& value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
};

#endif //_LOG_H_