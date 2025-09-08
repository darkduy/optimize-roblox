// include/common/BaseOptimizer.h - Base class definition
#pragma once

#include <string>
#include <cstdint>

struct ProcessInfo {
    uint32_t pid;
    std::string name;
    uint64_t memoryUsage;
    double cpuUsage;
    bool isRunning;
    
    ProcessInfo() : pid(0), memoryUsage(0), cpuUsage(0.0), isRunning(false) {}
};

struct OptimizationResult {
    bool success;
    std::string message;
    std::string details;
    
    OptimizationResult() : success(false) {}
    OptimizationResult(bool s, const std::string& m, const std::string& d = "") 
        : success(s), message(m), details(d) {}
};

class BaseOptimizer {
protected:
    bool isOptimizing;
    
public:
    BaseOptimizer() : isOptimizing(false) {}
    virtual ~BaseOptimizer() = default;
    
    // Pure virtual methods - platform specific
    virtual bool findRobloxProcess() = 0;
    virtual OptimizationResult optimizeProcessPriority() = 0;
    virtual OptimizationResult optimizeMemory() = 0;
    virtual OptimizationResult optimizeSystemSettings() = 0;
    virtual ProcessInfo getProcessInfo() = 0;
    
    // Common methods
    virtual void startOptimization() { isOptimizing = true; }
    virtual void stopOptimization() { isOptimizing = false; }
    virtual bool isRunning() const { return isOptimizing; }
};
