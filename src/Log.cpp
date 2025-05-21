#include "Log.h"

LogLevel Log::configLogLevel {LogLevel::INFO};
std::ofstream Log::logFile;

void Log::setLogFile(std::string fileName) {
    logFile.open(fileName, std::ios::app);
}

void Log::setLogLevel(LogLevel level) {
    configLogLevel = level;
}

std::string Log::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Log::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void Log::log(LogLevel level, const std::string& message) {
    std::string output = "[" + getTimestamp() + "] [" + levelToString(level) + "] " + message;

    std::string color;
    switch (level) {
        case LogLevel::DEBUG:   color = COLOR_BLUE; break;
        case LogLevel::INFO:    color = COLOR_WHITE; break;
        case LogLevel::WARNING: color = COLOR_YELLOW; break;
        case LogLevel::ERROR:   color = COLOR_RED; break;
        default:                color = COLOR_WHITE; break;
    }

    if (logFile.is_open()) {
        logFile << output << std::endl;
    }

    if (level == LogLevel::DEBUG && configLogLevel != LogLevel::DEBUG) { return; }

    std::cout << color << output << COLOR_RESET << std::endl;
}

// void Log::debug(const std::string& msg) {
//     log(LogLevel::DEBUG, msg);
// }

// void Log::info(const std::string& msg) {
//     log(LogLevel::INFO, msg);
// }

// void Log::warning(const std::string& msg) {
//     log(LogLevel::WARNING, msg);
// }

// void Log::error(const std::string& msg) {
//     log(LogLevel::ERROR, msg);
// }
