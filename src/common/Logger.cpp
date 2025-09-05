// src/common/Logger.cpp
#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>

Logger* Logger::instance{nullptr};
std::mutex Logger::mutex_;

Logger::Logger(const std::string& filename) : currentLevel(LogLevel::INFO) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger* Logger::getInstance(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr) {
        instance = new Logger(filename);
    }
    return instance;
}

void Logger::setLevel(LogLevel level) {
    currentLevel = level;
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < currentLevel) return;
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    std::string levelStr;
    switch (level) {
        case LogLevel::DEBUG: levelStr = "DEBUG"; break;
        case LogLevel::INFO: levelStr = "INFO"; break;
        case LogLevel::WARNING: levelStr = "WARN"; break;
        case LogLevel::ERROR: levelStr = "ERROR"; break;
    }
    
    std::string logLine = "[" + oss.str() + "] [" + levelStr + "] " + message;
    
    // Output to console
    std::cout << logLine << std::endl;
    
    // Output to file
    if (logFile.is_open()) {
        logFile << logLine << std::endl;
        logFile.flush();
    }
}

void Logger::debug(const std::string& message) { log(LogLevel::DEBUG, message); }
void Logger::info(const std::string& message) { log(LogLevel::INFO, message); }
void Logger::warning(const std::string& message) { log(LogLevel::WARNING, message); }
void Logger::error(const std::string& message) { log(LogLevel::ERROR, message); }
