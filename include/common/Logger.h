// include/common/Logger.h
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
    
    // Prevent cloning
    Logger(Logger &other) = delete;
    void operator=(const Logger &) = delete;
};

// Macro definitions for easy logging
#define LOG_DEBUG(msg) Logger::getInstance()->debug(msg)
#define LOG_INFO(msg) Logger::getInstance()->info(msg)
#define LOG_WARNING(msg) Logger::getInstance()->warning(msg)
#define LOG_ERROR(msg) Logger::getInstance()->error(msg)

---

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
    static WindowsProcessInfo getProcessInfo(DWORD pid);
    static bool terminateProcess(DWORD pid);
    static bool suspendProcess(DWORD pid);
    static bool resumeProcess(DWORD pid);
    
    // Process monitoring
    static double getCpuUsage(DWORD pid);
    static SIZE_T getMemoryUsage(DWORD pid);
    static std::vector<DWORD> getThreadIds(DWORD pid);
    
private:
    static bool enableDebugPrivilege();
};

---

// include/windows/RegistryManager.h
#pragma once
#ifdef WINDOWS_BUILD

#include <windows.h>
#include <string>

class RegistryManager {
public:
    // Registry operations
    static bool writeString(HKEY hKey, const std::wstring& subKey, 
                          const std::wstring& valueName, const std::wstring& value);
    static bool writeDword(HKEY hKey, const std::wstring& subKey, 
                         const std::wstring& valueName, DWORD value);
    static std::wstring readString(HKEY hKey, const std::wstring& subKey, 
                                 const std::wstring& valueName, const std::wstring& defaultValue = L"");
    static DWORD readDword(HKEY hKey, const std::wstring& subKey, 
                         const std::wstring& valueName, DWORD defaultValue = 0);
    
    // Gaming optimizations
    static bool enableGameMode();
    static bool disableGameBar();
    static bool optimizeVisualEffects();
    static bool disableWindowsDefenderRealTime();
    static bool optimizePowerSettings();
    
    // Backup and restore
    static bool backupKey(HKEY hKey, const std::wstring& subKey, const std::wstring& backupFile);
    static bool restoreKey(HKEY hKey, const std::wstring& subKey, const std::wstring& backupFile);
    
private:
    static bool openKey(HKEY hKey, const std::wstring& subKey, HKEY& result, 
                       REGSAM access = KEY_READ | KEY_WRITE);
};

---

// include/android/AndroidOptimizer.h
#pragma once
#ifdef ANDROID_BUILD

#include "BaseOptimizer.h"
#include <jni.h>

class AndroidOptimizer : public BaseOptimizer {
private:
    JavaVM* jvm;
    jobject activityObject;
    std::string packageName;
    
public:
    AndroidOptimizer();
    ~AndroidOptimizer() override;
    
    // Inherited from BaseOptimizer  
    bool findRobloxProcess() override;
    OptimizationResult optimizeProcessPriority() override;
    OptimizationResult optimizeMemory() override;
    OptimizationResult optimizeSystemSettings() override;
    ProcessInfo getProcessInfo() override;
    
    // Android-specific methods
    bool setJavaVM(JavaVM* vm);
    bool setActivityObject(jobject activity);
    OptimizationResult optimizeCpuGovernor();
    OptimizationResult optimizeGpuFrequency();
    OptimizationResult clearCache();
    OptimizationResult optimizeBatterySettings();
    
private:
    JNIEnv* getJNIEnv();
    bool callJavaMethod(const char* methodName, const char* signature, ...);
};

---

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
