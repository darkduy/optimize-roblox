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
