// src/windows/ProcessManager.cpp - Process management stub
#ifdef WINDOWS_BUILD
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>

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
        if (hProcess) {
            bool result = SetPriorityClass(hProcess, priority);
            CloseHandle(hProcess);
            return result;
        }
        return false;
    }
};

#endif // WINDOWS_BUILD
