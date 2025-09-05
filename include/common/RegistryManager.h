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
