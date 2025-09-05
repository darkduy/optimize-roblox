// include/common/Config.h
#pragma once
#include <string>
#include <map>
#include <mutex>

class Config {
private:
    static Config* instance;
    static std::mutex mutex_;
    std::map<std::string, std::string> settings;
    std::string configFile;
    
protected:
    Config();

public:
    static Config* getInstance();
    ~Config();
    
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename = "");
    
    // Getters
    std::string getString(const std::string& key, const std::string& defaultValue = "");
    int getInt(const std::string& key, int defaultValue = 0);
    bool getBool(const std::string& key, bool defaultValue = false);
    double getDouble(const std::string& key, double defaultValue = 0.0);
    
    // Setters
    void setString(const std::string& key, const std::string& value);
    void setInt(const std::string& key, int value);
    void setBool(const std::string& key, bool value);
    void setDouble(const std::string& key, double value);
    
    // Utility
    bool hasKey(const std::string& key) const;
    void removeKey(const std::string& key);
    void clear();
    
    // Prevent cloning
    Config(Config &other) = delete;
    void operator=(const Config &) = delete;
};
