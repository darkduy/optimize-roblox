// src/common/Utils.cpp
#include "Utils.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iomanip>

#ifdef WINDOWS_BUILD
#include <windows.h>
#include <shlobj.h>
#elif defined(ANDROID_BUILD)
#include <unistd.h>
#include <sys/stat.h>
#elif defined(LINUX_BUILD)
#include <unistd.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#endif

std::string Utils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) return str;
    
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> Utils::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    
    return tokens;
}

std::string Utils::toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string Utils::toUpperCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

bool Utils::fileExists(const std::string& path) {
#ifdef WINDOWS_BUILD
    DWORD attrs = GetFileAttributesA(path.c_str());
    return (attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
#endif
}

std::string Utils::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

bool Utils::writeFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) return false;
    
    file << content;
    return file.good();
}

std::string Utils::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
    return oss.str();
}

uint64_t Utils::getCurrentTimeMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string Utils::getExecutablePath() {
#ifdef WINDOWS_BUILD
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    return std::string(path);
#else
    char path[1024];
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (count != -1) {
        path[count] = '\0';
        return std::string(path);
    }
    return "";
#endif
}

std::string Utils::getConfigDirectory() {
#ifdef WINDOWS_BUILD
    char path[MAX_PATH];
    if (SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path) == S_OK) {
        return std::string(path) + "\\RobloxOptimizer";
    }
#else
    const char* home = getenv("HOME");
    if (home) {
        return std::string(home) + "/.roblox-optimizer";
    }
#endif
    return "./config";
}

std::string Utils::formatBytes(uint64_t bytes) {
    const char* sizes[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024 && i < 4) {
        size /= 1024;
        i++;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << " " << sizes[i];
    return oss.str();
}

uint64_t Utils::getAvailableMemory() {
#ifdef WINDOWS_BUILD
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    return statex.ullAvailPhys;
#elif defined(LINUX_BUILD)
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return info.freeram * info.mem_unit;
    }
#endif
    return 0;
}
