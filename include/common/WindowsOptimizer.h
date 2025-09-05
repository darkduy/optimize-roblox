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
