// include/common/BaseOptimizer.h
#pragma once
#include <string>
#include <vector>
#include <functional>

struct ProcessInfo {
    uint32_t pid;
    std::string name;
    uint64_t memoryUsage;
    double cpuUsage;
    bool isRunning;
};

struct OptimizationResult {
    bool success;
    std::string message;
    std::string details;
};

class BaseOptimizer {
protected:
    bool isOptimizing;
    std::vector<std::function<void(const std::string&)>> statusCallbacks;
    
public:
    BaseOptimizer();
    virtual ~BaseOptimizer();
    
    // Pure virtual methods - must be implemented by platform
    virtual bool findRobloxProcess() = 0;
    virtual OptimizationResult optimizeProcessPriority() = 0;
    virtual OptimizationResult optimizeMemory() = 0;
    virtual OptimizationResult optimizeSystemSettings() = 0;
    virtual ProcessInfo getProcessInfo() = 0;
    
    // Common methods
    virtual void startOptimization();
    virtual void stopOptimization();
    virtual bool isRunning() const;
    
    // Callback registration
    void registerStatusCallback(std::function<void(const std::string&)> callback);
    void clearStatusCallbacks();
    
protected:
    void notifyStatusChange(const std::string& status);
};
