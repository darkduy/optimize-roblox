// src/windows/RegistryManager.cpp - Registry management stub
#ifdef WINDOWS_BUILD
#include <windows.h>
#include <string>

class RegistryManager {
public:
    static bool enableGameMode() {
        HKEY hKey;
        DWORD value = 1;
        
        LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, 
                                  L"Software\\Microsoft\\GameBar", 
                                  0, KEY_SET_VALUE, &hKey);
        
        if (result == ERROR_SUCCESS) {
            RegSetValueEx(hKey, L"AllowAutoGameMode", 0, REG_DWORD, 
                         (BYTE*)&value, sizeof(DWORD));
            RegCloseKey(hKey);
            return true;
        }
        
        return false;
    }
    
    static bool optimizeVisualEffects() {
        HKEY hKey;
        DWORD value = 2; // Performance mode
        
        LONG result = RegOpenKeyEx(HKEY_CURRENT_USER,
                                  L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VisualEffects",
                                  0, KEY_SET_VALUE, &hKey);
        
        if (result == ERROR_SUCCESS) {
            RegSetValueEx(hKey, L"VisualFXSetting", 0, REG_DWORD,
                         (BYTE*)&value, sizeof(DWORD));
            RegCloseKey(hKey);
            return true;
        }
        
        return false;
    }
};

#endif // WINDOWS_BUILD
