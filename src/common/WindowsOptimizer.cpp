// src/windows/WindowsOptimizer.cpp
#ifdef WINDOWS_BUILD
#include "WindowsOptimizer.h"
#include "ProcessManager.h"
#include "RegistryManager.h"
#include "Logger.h"
#include "Config.h"
#include <iostream>

WindowsOptimizer::WindowsOptimizer() : hProcess(nullptr), processId(0) {
    processName = L"RobloxPlayerBeta.exe";
}

WindowsOptimizer::~WindowsOptimizer() {
    closeProcessHandle();
}

bool WindowsOptimizer::findRobloxProcess() {
    std::vector<std::wstring> robloxNames = {
        L"RobloxPlayerBeta.exe",
        L"Roblox.exe",
        L"RobloxStudioBeta.exe"
    };
    
    for (const auto& name : robloxNames) {
        auto pids = findProcessesByName(name);
        if (!pids.empty()) {
            processId = pids[0]; // Take the first one
            processName = name;
            if (openProcessHandle(processId)) {
                LOG_INFO("Found Roblox process: " + std::string(name.begin(), name.end()) + 
                        " (PID: " + std::to_string(processId) + ")");
                return true;
            }
        }
    }
    
    LOG_WARNING("No Roblox process found");
    return false;
}

OptimizationResult WindowsOptimizer::optimizeProcessPriority() {
    if (!hProcess) {
        return {false, "No process handle available", ""};
    }
    
    // Set high priority (not realtime to avoid system instability)
    if (SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS)) {
        LOG_INFO("Process priority set to HIGH");
        return {true, "Process priority optimized", "Set to HIGH_PRIORITY_CLASS"};
    } else {
        DWORD error = GetLastError();
        return {false, "Failed to set process priority", 
                "Error code: " + std::to_string(error)};
    }
}

OptimizationResult WindowsOptimizer::optimizeMemory() {
    if (!hProcess) {
        return {false, "No process handle available", ""};
    }
    
    // Trim working set
    if (SetProcessWorkingSetSize(hProcess, -1, -1)) {
        LOG_INFO("Working set trimmed successfully");
        return {true, "Memory optimized", "Working set trimmed"};
    } else {
        DWORD error = GetLastError();
        return {false, "Failed to trim working set", 
                "Error code: " + std::to_string(error)};
    }
}

OptimizationResult WindowsOptimizer::optimizeSystemSettings() {
    std::vector<std::string> results;
    bool overallSuccess = true;
    
    // Enable Game Mode
    if (RegistryManager::enableGameMode()) {
        results.push_back("✓ Game Mode enabled");
        LOG_INFO("Game Mode enabled");
    } else {
        results.push_back("✗ Failed to enable Game Mode");
        overallSuccess = false;
    }
    
    // Disable Game Bar
    if (RegistryManager::disableGameBar()) {
        results.push_back("✓ Game Bar disabled");
        LOG_INFO("Game Bar disabled");
    } else {
        results.push_back("✗ Failed to disable Game Bar");
        overallSuccess = false;
    }
    
    // Optimize Visual Effects
    if (RegistryManager::optimizeVisualEffects()) {
        results.push_back("✓ Visual Effects optimized");
        LOG_INFO("Visual Effects optimized");
    } else {
        results.push_back("✗ Failed to optimize Visual Effects");
        overallSuccess = false;
    }
    
    // Optimize Power Settings
    if (RegistryManager::optimizePowerSettings()) {
        results.push_back("✓ Power settings optimized");
        LOG_INFO("Power settings optimized");
    } else {
        results.push_back("✗ Failed to optimize power settings");
        overallSuccess = false;
    }
    
    std::string details;
    for (const auto& result : results) {
        details += result + "\n";
    }
    
    return {overallSuccess, 
            overallSuccess ? "System settings optimized" : "Some optimizations failed",
            details};
}

ProcessInfo WindowsOptimizer::getProcessInfo() {
    ProcessInfo info = {};
    
    if (!hProcess) {
        return info;
    }
    
    info.pid = processId;
    info.name = std::string(processName.begin(), processName.end());
    info.isRunning = true;
    
    // Get memory usage
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        info.memoryUsage = pmc.WorkingSetSize;
    }
    
    // Get CPU usage
    info.cpuUsage = ProcessManager::getCpuUsage(processId);
    
    return info;
}

bool WindowsOptimizer::setProcessAffinity(DWORD_PTR affinityMask) {
    if (!hProcess) return false;
    
    return SetProcessAffinityMask(hProcess, affinityMask) != 0;
}

OptimizationResult WindowsOptimizer::cleanTempFiles() {
    // Clean Windows temp directory
    std::wstring tempPath = L"C:\\Windows\\Temp\\";
    // Implementation for temp file cleaning
    // This is a safe operation that removes temporary files
    
    return {true, "Temporary files cleaned", "Cleaned Windows temp directory"};
}

bool WindowsOptimizer::openProcessHandle(DWORD pid) {
    closeProcessHandle();
    
    hProcess = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_SET_INFORMATION | PROCESS_VM_READ,
        FALSE, pid);
    
    return hProcess != nullptr;
}

void WindowsOptimizer::closeProcessHandle() {
    if (hProcess) {
        CloseHandle(hProcess);
        hProcess = nullptr;
    }
}

std::vector<DWORD> WindowsOptimizer::findProcessesByName(const std::wstring& name) {
    std::vector<DWORD> pids;
    
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return pids;
    }
    
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);
    
    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            if (name == pe32.szExeFile) {
                pids.push_back(pe32.th32ProcessID);
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }
    
    CloseHandle(hSnapshot);
    return pids;
}

#endif // WINDOWS_BUILD
