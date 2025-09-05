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

---

// include/common/Config.h
#pragma once
#include <string>
#include <map>
#include <mutex>

class Config {
private:
    static Config* instance;
    static std::mutex mutex_;
    std::map<std::string, std::string> settings;
    std::string configFile;
    
protected:
    Config();

public:
    static Config* getInstance();
    ~Config();
    
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename = "");
    
    // Getters
    std::string getString(const std::string& key, const std::string& defaultValue = "");
    int getInt(const std::string& key, int defaultValue = 0);
    bool getBool(const std::string& key, bool defaultValue = false);
    double getDouble(const std::string& key, double defaultValue = 0.0);
    
    // Setters
    void setString(const std::string& key, const std::string& value);
    void setInt(const std::string& key, int value);
    void setBool(const std::string& key, bool value);
    void setDouble(const std::string& key, double value);
    
    // Utility
    bool hasKey(const std::string& key) const;
    void removeKey(const std::string& key);
    void clear();
    
    // Prevent cloning
    Config(Config &other) = delete;
    void operator=(const Config &) = delete;
};

---

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

---

// include/windows/WindowsOptimizer.h
#pragma once
#ifdef WINDOWS_BUILD

#include "BaseOptimizer.h"
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

class WindowsOptimizer : public BaseOptimizer {
private:
    HANDLE hProcess;
    DWORD processId;
    std::wstring processName;
    
public:
    WindowsOptimizer();
    ~WindowsOptimizer() override;
    
    // Inherited from BaseOptimizer
    bool findRobloxProcess() override;
    OptimizationResult optimizeProcessPriority() override;
    OptimizationResult optimizeMemory() override;
    OptimizationResult optimizeSystemSettings() override;
    ProcessInfo getProcessInfo() override;
    
    // Windows-specific methods
    bool setProcessAffinity(DWORD_PTR affinityMask);
    OptimizationResult optimizeWindowsGameMode();
    OptimizationResult optimizeVisualEffects();
    OptimizationResult cleanTempFiles();
    
private:
    bool openProcessHandle(DWORD pid);
    void closeProcessHandle();
    std::vector<DWORD> findProcessesByName(const std::wstring& name);
};

---

// include/windows/ProcessManager.h
#pragma once
#ifdef WINDOWS_BUILD

#include <windows.h>
#include <vector>
#include <string>

struct WindowsProcessInfo {
    DWORD pid;
    std::wstring name;
    std::wstring path;
    DWORD parentPid;
    DWORD threadCount;
    SIZE_T workingSetSize;
    SIZE_T privateBytes;
};

class ProcessManager {
public:
    static std::vector<WindowsProcessInfo> getProcessList();
