// src/common/Config.cpp
#include "Config.h"
#include "Utils.h"
#include <fstream>
#include <sstream>

Config* Config::instance{nullptr};
std::mutex Config::mutex_;

Config::Config() {
    configFile = Utils::getConfigDirectory() + "/config.ini";
    loadFromFile(configFile);
}

Config::~Config() {
    saveToFile();
}

Config* Config::getInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr) {
        instance = new Config();
    }
    return instance;
}

bool Config::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        line = Utils::trim(line);
        
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }
        
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = Utils::trim(line.substr(0, pos));
            std::string value = Utils::trim(line.substr(pos + 1));
            settings[key] = value;
        }
    }
    
    return true;
}

bool Config::saveToFile(const std::string& filename) {
    std::string file = filename.empty() ? configFile : filename;
    std::ofstream outFile(file);
    
    if (!outFile.is_open()) {
        return false;
    }
    
    outFile << "# Roblox Optimizer Configuration\n";
    outFile << "# Generated at: " << Utils::getCurrentTimestamp() << "\n\n";
    
    for (const auto& pair : settings) {
        outFile << pair.first << "=" << pair.second << "\n";
    }
    
    return outFile.good();
}

std::string Config::getString(const std::string& key, const std::string& defaultValue) {
    auto it = settings.find(key);
    return (it != settings.end()) ? it->second : defaultValue;
}

int Config::getInt(const std::string& key, int defaultValue) {
    auto it = settings.find(key);
    if (it != settings.end()) {
        try {
            return std::stoi(it->second);
        } catch (const std::exception&) {
            return defaultValue;
        }
    }
    return defaultValue;
}

bool Config::getBool(const std::string& key, bool defaultValue) {
    auto it = settings.find(key);
    if (it != settings.end()) {
        std::string value = Utils::toLowerCase(it->second);
        return (value == "true" || value == "1" || value == "yes" || value == "on");
    }
    return defaultValue;
}

double Config::getDouble(const std::string& key, double defaultValue) {
    auto it = settings.find(key);
    if (it != settings.end()) {
        try {
            return std::stod(it->second);
        } catch (const std::exception&) {
            return defaultValue;
        }
    }
    return defaultValue;
}

void Config::setString(const std::string& key, const std::string& value) {
    settings[key] = value;
}

void Config::setInt(const std::string& key, int value) {
    settings[key] = std::to_string(value);
}

void Config::setBool(const std::string& key, bool value) {
    settings[key] = value ? "true" : "false";
}

void Config::setDouble(const std::string& key, double value) {
    settings[key] = std::to_string(value);
}

bool Config::hasKey(const std::string& key) const {
    return settings.find(key) != settings.end();
}

void Config::removeKey(const std::string& key) {
    settings.erase(key);
}

void Config::clear() {
    settings.clear();
}
