// include/common/Logger.h - Logger class definition  
#pragma once

#include <string>
#include <fstream>
#include <mutex>

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3
};

class Logger {
private:
    static Logger* instance;
    static std::mutex mutex_;
    std::ofstream logFile;
    LogLevel currentLevel;
    
protected:
    Logger(const std::string& filename = "optimizer.log");

public:
    static Logger* getInstance(const std::string& filename = "optimizer.log");
    ~Logger();
    
    void setLevel(LogLevel level);
    void log(LogLevel level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    
    Logger(Logger &other) = delete;
    void operator=(const Logger &) = delete;
};

// Convenience macros
#define LOG_DEBUG(msg) Logger::getInstance()->debug(msg)
#define LOG_INFO(msg) Logger::getInstance()->info(msg)  
#define LOG_WARNING(msg) Logger::getInstance()->warning(msg)
#define LOG_ERROR(msg) Logger::getInstance()->error(msg)
