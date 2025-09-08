// include/android/SystemManager.h
#pragma once
#ifdef ANDROID_BUILD

#include <string>
#include <vector>

struct AndroidAppInfo {
    std::string packageName;
    std::string appName;
    int uid;
    long memoryUsage;
    bool isRunning;
};

class SystemManager {
public:
    static std::vector<AndroidAppInfo> getRunningApps();
    static AndroidAppInfo getAppInfo(const std::string& packageName);
    
    // System optimization
    static bool setCpuGovernor(const std::string& governor);
    static std::string getCurrentCpuGovernor();
    static bool setGpuGovernor(const std::string& governor);
    
    // Memory management
    static long getTotalMemory();
    static long getAvailableMemory();
    static bool trimMemory(const std::string& packageName);
    
    // Performance
    static bool disableAnimations();
    static bool enablePerformanceMode();
    static bool optimizeBattery();
    
private:
    static bool hasRootAccess();
    static bool executeCommand(const std::string& command);
};

#endif // ANDROID_BUILD
