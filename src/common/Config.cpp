// src/common/Config.cpp - Basic implementation
#include <map>
#include <string>
#include <fstream>

class Config {
private:
    static Config* instance;
    std::map<std::string, std::string> settings;
    
public:
    static Config* getInstance() {
        if (!instance) {
            instance = new Config();
        }
        return instance;
    }
    
    std::string getString(const std::string& key, const std::string& defaultValue = "") {
        auto it = settings.find(key);
        return (it != settings.end()) ? it->second : defaultValue;
    }
    
    void setString(const std::string& key, const std::string& value) {
        settings[key] = value;
    }
    
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }
};

Config* Config::instance = nullptr;
