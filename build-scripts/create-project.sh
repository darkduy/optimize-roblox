#!/bin/bash
# create-project.sh - Tạo cấu trúc project và files

set -e

echo "🚀 Creating Roblox Optimizer project structure..."

# Create directory structure
mkdir -p src/{common,windows,android}
mkdir -p include/{common,windows,android}
mkdir -p build-scripts

# Common source files
cat > src/common/Logger.cpp << 'EOF'
#include <iostream>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>

class Logger {
private:
    static std::mutex mtx;
    std::ofstream logFile;
    
public:
    Logger() {
        logFile.open("optimizer.log", std::ios::app);
    }
    
    void info(const std::string& message) {
        std::lock_guard<std::mutex> lock(mtx);
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        std::cout << "[INFO] " << message << std::endl;
        if (logFile.is_open()) {
            logFile << "[INFO] " << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") 
                   << " " << message << std::endl;
        }
    }
    
    void error(const std::string& message) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[ERROR] " << message << std::endl;
        if (logFile.is_open()) {
            logFile << "[ERROR] " << message << std::endl;
        }
    }
    
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
};

std::mutex Logger::mtx;

// C interface for simple usage
extern "C" void log_info(const char* message) {
    Logger::getInstance().info(std::string(message));
}

extern "C" void log_error(const char* message) {
    Logger::getInstance().error(std::string(message));
}
EOF

cat > src/common/Utils.cpp << 'EOF'
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdint>

class Utils {
public:
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
    
    static std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(trim(token));
        }
        return tokens;
    }
    
    static std::string formatBytes(uint64_t bytes) {
        const char* units[] = {"B", "KB", "MB", "GB", "TB"};
        int unit = 0;
        double size = static_cast<double>(bytes);
        while (size >= 1024 && unit < 4) {
            size /= 1024;
            unit++;
        }
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%.1f %s", size, units[unit]);
        return std::string(buffer);
    }
    
    static std::string toLowerCase(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
    
    static uint64_t getCurrentTimeMillis() {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    }
};

// C interface
extern "C" const char* format_bytes(uint64_t bytes) {
    static std::string result = Utils::formatBytes(bytes);
    return result.c_str();
}
EOF

cat > src/common/Config.cpp << 'EOF'
#include <map>
#include <string>
#include <fstream>
#include <iostream>

class Config {
private:
    std::map<std::string, std::string> settings;
    std::string configFile;
    
public:
    Config() : configFile("config.ini") {}
    
    bool load() {
        std::ifstream file(configFile);
        if (!file.is_open()) {
            return false;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                settings[key] = value;
            }
        }
        return true;
    }
    
    std::string getString(const std::string& key, const std::string& defaultValue = "") {
        auto it = settings.find(key);
        return (it != settings.end()) ? it->second : defaultValue;
    }
    
    void setString(const std::string& key, const std::string& value) {
        settings[key] = value;
    }
    
    bool save() {
        std::ofstream file(configFile);
        if (!file.is_open()) {
            return false;
        }
        
        for (const auto& pair : settings) {
            file << pair.first << "=" << pair.second << std::endl;
        }
        return true;
    }
    
    static Config& getInstance() {
        static Config instance;
        return instance;
    }
};

// C interface
extern "C" const char* config_get_string(const char* key) {
    static std::string result = Config::getInstance().getString(std::string(key));
    return result.c_str();
}

extern "C" void config_set_string(const char* key, const char* value) {
    Config::getInstance().setString(std::string(key), std::string(value));
}
EOF

# Windows source files
cat > src/windows/main_windows.cpp << 'EOF'
#ifdef WINDOWS_BUILD
#include <iostream>
#include <windows.h>
#include <string>

extern void log_info(const char* message);

int main() {
    // Set UTF-8 console output
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    std::wcout.imbue(std::locale(""));
    
    // Header
    std::wcout << L"╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::wcout << L"║                 ROBLOX SYSTEM OPTIMIZER v1.0                ║" << std::endl;
    std::wcout << L"║                    Windows 10/11 Edition                    ║" << std::endl;
    std::wcout << L"╠══════════════════════════════════════════════════════════════╣" << std::endl;
    std::wcout << L"║ 🎮 Tối ưu hóa hiệu năng Roblox một cách an toàn             ║" << std::endl;
    std::wcout << L"║ 🛡️  Không chỉnh sửa game files - Hoàn toàn hợp pháp!       ║" << std::endl;
    std::wcout << L"║ ✅ Chỉ hỗ trợ Windows 10/11 x64                            ║" << std::endl;
    std::wcout << L"╚══════════════════════════════════════════════════════════════╝" << std::endl;
    std::wcout << std::endl;
    
    log_info("Roblox Optimizer started successfully");
    log_info("Platform: Windows 10/11 x64");
    log_info("Build: Release candidate");
    
    std::wcout << L"📊 Thông tin hệ thống:" << std::endl;
    
    // Get Windows version
    OSVERSIONINFOW osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOW));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
    
    #pragma warning(push)
    #pragma warning(disable: 4996)
    if (GetVersionExW(&osvi)) {
        std::wcout << L"   Windows Version: " << osvi.dwMajorVersion << L"." << osvi.dwMinorVersion << std::endl;
    }
    #pragma warning(pop)
    
    // Get system info
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::wcout << L"   Processor Count: " << sysInfo.dwNumberOfProcessors << std::endl;
    
    // Get memory info
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    std::wcout << L"   Total RAM: " << (memInfo.ullTotalPhys / 1024 / 1024 / 1024) << L" GB" << std::endl;
    std::wcout << L"   Available RAM: " << (memInfo.ullAvailPhys / 1024 / 1024 / 1024) << L" GB" << std::endl;
    
    std::wcout << std::endl;
    std::wcout << L"🚀 Ready to optimize Roblox!" << std::endl;
    std::wcout << L"   Note: This is a build test version." << std::endl;
    std::wcout << L"   Full optimization features coming soon!" << std::endl;
    std::wcout << std::endl;
    std::wcout << L"Press Enter to exit...";
    std::wcin.get();
    
    log_info("Roblox Optimizer stopped");
    return 0;
}
#endif // WINDOWS_BUILD
EOF

cat > src/windows/WindowsOptimizer.cpp << 'EOF'
#ifdef WINDOWS_BUILD
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iostream>
#include <vector>
#include <string>

extern void log_info(const char* message);
extern void log_error(const char* message);

class WindowsOptimizer {
private:
    HANDLE hProcess;
    DWORD processId;
    
public:
    WindowsOptimizer() : hProcess(nullptr), processId(0) {
        log_info("WindowsOptimizer initialized");
    }
    
    ~WindowsOptimizer() {
        closeProcess();
    }
    
    std::vector<DWORD> findRobloxProcesses() {
        std::vector<DWORD> robloxPids;
        
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            log_error("Failed to create process snapshot");
            return robloxPids;
        }
        
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        
        if (Process32First(hSnapshot, &pe32)) {
            do {
                std::string processName = pe32.szExeFile;
                if (processName == "RobloxPlayerBeta.exe" || 
                    processName == "Roblox.exe" ||
                    processName == "RobloxStudioBeta.exe") {
                    robloxPids.push_back(pe32.th32ProcessID);
                    log_info(("Found Roblox process: " + processName + " (PID: " + std::to_string(pe32.th32ProcessID) + ")").c_str());
                }
            } while (Process32Next(hSnapshot, &pe32));
        }
        
        CloseHandle(hSnapshot);
        return robloxPids;
    }
    
    bool optimizeProcess(DWORD pid) {
        hProcess = OpenProcess(PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (!hProcess) {
            log_error("Failed to open Roblox process");
            return false;
        }
        
        processId = pid;
        
        // Set high priority
        if (SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS)) {
            log_info("Set Roblox process to HIGH priority");
        } else {
            log_error("Failed to set process priority");
        }
        
        // Trim working set
        if (SetProcessWorkingSetSize(hProcess, -1, -1)) {
            log_info("Trimmed Roblox process working set");
        } else {
            log_error("Failed to trim working set");
        }
        
        return true;
    }
    
    void getProcessInfo() {
        if (!hProcess) return;
        
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
            log_info(("Roblox memory usage: " + std::to_string(pmc.WorkingSetSize / 1024 / 1024) + " MB").c_str());
        }
    }
    
private:
    void closeProcess() {
        if (hProcess) {
            CloseHandle(hProcess);
            hProcess = nullptr;
            processId = 0;
        }
    }
};

// Global optimizer instance
static WindowsOptimizer* g_optimizer = nullptr;

extern "C" __declspec(dllexport) void* createOptimizer() {
    if (!g_optimizer) {
        g_optimizer = new WindowsOptimizer();
    }
    return g_optimizer;
}

extern "C" __declspec(dllexport) void optimizeRoblox() {
    if (!g_optimizer) {
        g_optimizer = new WindowsOptimizer();
    }
    
    auto pids = g_optimizer->findRobloxProcesses();
    if (pids.empty()) {
        log_error("No Roblox processes found");
        return;
    }
    
    for (DWORD pid : pids) {
        g_optimizer->optimizeProcess(pid);
        g_optimizer->getProcessInfo();
    }
}

#endif // WINDOWS_BUILD
EOF

cat > src/windows/ProcessManager.cpp << 'EOF'
#ifdef WINDOWS_BUILD
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <vector>

extern void log_info(const char* message);
extern void log_error(const char* message);

class ProcessManager {
public:
    static std::vector<DWORD> findProcessesByName(const std::string& name) {
        std::vector<DWORD> pids;
        
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            return pids;
        }
        
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        
        if (Process32First(hSnapshot, &pe32)) {
            do {
                if (name == pe32.szExeFile) {
                    pids.push_back(pe32.th32ProcessID);
                }
            } while (Process32Next(hSnapshot, &pe32));
        }
        
        CloseHandle(hSnapshot);
        return pids;
    }
    
    static bool setProcessPriority(DWORD pid, DWORD priority) {
        HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid);
        if (!hProcess) {
            return false;
        }
        
        bool result = SetPriorityClass(hProcess, priority);
        CloseHandle(hProcess);
        return result;
    }
    
    static SIZE_T getProcessMemoryUsage(DWORD pid) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (!hProcess) {
            return 0;
        }
        
        PROCESS_MEMORY_COUNTERS pmc;
        SIZE_T memoryUsage = 0;
        
        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
            memoryUsage = pmc.WorkingSetSize;
        }
        
        CloseHandle(hProcess);
        return memoryUsage;
    }
};

#endif // WINDOWS_BUILD
EOF

cat > src/windows/RegistryManager.cpp << 'EOF'
#ifdef WINDOWS_BUILD
#include <windows.h>
#include <string>

extern void log_info(const char* message);
extern void log_error(const char* message);

class RegistryManager {
public:
    static bool enableGameMode() {
        HKEY hKey;
        DWORD value = 1;
        
        LONG result = RegOpenKeyExW(HKEY_CURRENT_USER, 
                                   L"Software\\Microsoft\\GameBar", 
                                   0, KEY_SET_VALUE, &hKey);
        
        if (result == ERROR_SUCCESS) {
            RegSetValueExW(hKey, L"AllowAutoGameMode", 0, REG_DWORD, 
                          (BYTE*)&value, sizeof(DWORD));
            RegCloseKey(hKey);
            log_info("Enabled Windows Game Mode");
            return true;
        }
        
        log_error("Failed to enable Game Mode - registry access denied");
        return false;
    }
    
    static bool optimizeVisualEffects() {
        HKEY hKey;
        DWORD value = 2; // Best Performance
        
        LONG result = RegOpenKeyExW(HKEY_CURRENT_USER,
                                   L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Vis
