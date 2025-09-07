// src/common/Logger.cpp - Basic implementation
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
    
    void info(const std::string& message) {
        std::cout << "[INFO] " << message << std::endl;
    }
    
    void error(const std::string& message) {
        std::cout << "[ERROR] " << message << std::endl;
    }
};
