// src/android/SystemManager.cpp - Android system management
#ifdef ANDROID_BUILD
#include <android/log.h>
#include <sys/system_properties.h>
#include <fstream>
#include <string>
#include <vector>

#define LOG_TAG "SystemManager"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

class SystemManager {
public:
    static bool hasRootAccess() {
        return (geteuid() == 0) || system("su -c 'echo test' 2>/dev/null") == 0;
    }
    
    static std::string getCurrentCpuGovernor() {
        std::ifstream file("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");
        if (file.is_open()) {
            std::string governor;
            std::getline(file, governor);
            return governor;
        }
        return "unknown";
    }
    
    static bool setCpuGovernor(const std::string& governor) {
        if (!hasRootAccess()) {
            LOGI("Root access required for CPU governor change");
            return false;
        }
        
        std::string command = "echo " + governor + " > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor";
        return system(command.c_str()) == 0;
    }
    
    static long getTotalMemory() {
        std::ifstream meminfo("/proc/meminfo");
        if (meminfo.is_open()) {
            std::string line;
            while (std::getline(meminfo, line)) {
                if (line.find("MemTotal:") == 0) {
                    // Extract memory value
                    size_t start = line.find_first_of("0123456789");
                    size_t end = line.find(" kB");
                    if (start != std::string::npos && end != std::string::npos) {
                        long kb = std::stol(line.substr(start, end - start));
                        return kb * 1024; // Convert to bytes
                    }
                }
            }
        }
        return 0;
    }
    
    static long getAvailableMemory() {
        std::ifstream meminfo("/proc/meminfo");
        if (meminfo.is_open()) {
            std::string line;
            while (std::getline(meminfo, line)) {
                if (line.find("MemAvailable:") == 0) {
                    size_t start = line.find_first_of("0123456789");
                    size_t end = line.find(" kB");
                    if (start != std::string::npos && end != std::string::npos) {
                        long kb = std::stol(line.substr(start, end - start));
                        return kb * 1024; // Convert to bytes
                    }
                }
            }
        }
        return 0;
    }
    
    static bool disableAnimations() {
        if (!hasRootAccess()) {
            LOGI("Limited animation optimization without root");
            return false;
        }
        
        // Disable system animations
        std::vector<std::string> commands = {
            "settings put global window_animation_scale 0",
            "settings put global transition_animation_scale 0",
            "settings put global animator_duration_scale 0"
        };
        
        bool success = true;
        for (const auto& cmd : commands) {
            if (system(cmd.c_str()) != 0) {
                success = false;
            }
        }
        
        return success;
    }
    
    static bool optimizeBattery() {
        // Optimize battery settings for performance
        std::vector<std::string> commands = {
            "settings put global low_power 0",
            "settings put system screen_brightness_mode 0"
        };
        
        for (const auto& cmd : commands) {
            system(cmd.c_str()); // Don't fail if some commands don't work
        }
        
        return true;
    }
    
    static std::string getSystemInfo() {
        char brand[PROP_VALUE_MAX];
        char model[PROP_VALUE_MAX];
        char version[PROP_VALUE_MAX];
        char sdk[PROP_VALUE_MAX];
        
        __system_property_get("ro.product.brand", brand);
        __system_property_get("ro.product.model", model);
        __system_property_get("ro.build.version.release", version);
        __system_property_get("ro.build.version.sdk", sdk);
        
        std::string info = "Brand: ";
        info += brand;
        info += "\nModel: ";
        info += model;  
        info += "\nAndroid: ";
        info += version;
        info += "\nSDK: ";
        info += sdk;
        info += "\nRoot: ";
        info += hasRootAccess() ? "Yes" : "No";
        
        return info;
    }
};

#endif // ANDROID_BUILD
