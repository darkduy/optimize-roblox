// include/common/Utils.h
#pragma once
#include <string>
#include <vector>
#include <chrono>

class Utils {
public:
    // String utilities
    static std::string trim(const std::string& str);
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string toLowerCase(const std::string& str);
    static std::string toUpperCase(const std::string& str);
    
    // File utilities
    static bool fileExists(const std::string& path);
    static std::string readFile(const std::string& path);
    static bool writeFile(const std::string& path, const std::string& content);
    
    // Time utilities
    static std::string getCurrentTimestamp();
    static uint64_t getCurrentTimeMillis();
    
    // System utilities
    static std::string getExecutablePath();
    static std::string getConfigDirectory();
    
    // Memory utilities
    static std::string formatBytes(uint64_t bytes);
    static uint64_t getAvailableMemory();
};
