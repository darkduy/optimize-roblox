// src/android/AndroidOptimizer.cpp
#ifdef ANDROID_BUILD
#include "AndroidOptimizer.h"
#include "SystemManager.h"
#include "Logger.h"
#include <android/log.h>
#include <unistd.h>
#include <sys/system_properties.h>

AndroidOptimizer::AndroidOptimizer() : jvm(nullptr), activityObject(nullptr) {
    packageName = "com.roblox.client";
}

AndroidOptimizer::~AndroidOptimizer() {
    if (jvm && activityObject) {
        JNIEnv* env = getJNIEnv();
        if (env) {
            env->DeleteGlobalRef(activityObject);
        }
    }
}

bool AndroidOptimizer::setJavaVM(JavaVM* vm) {
    jvm = vm;
    return true;
}

bool AndroidOptimizer::setActivityObject(jobject activity) {
    if (!jvm) return false;
    
    JNIEnv* env = getJNIEnv();
    if (!env) return false;
    
    activityObject = env->NewGlobalRef(activity);
    return activityObject != nullptr;
}

bool AndroidOptimizer::findRobloxProcess() {
    auto apps = SystemManager::getRunningApps();
    
    for (const auto& app : apps) {
        if (app.packageName == packageName || 
            app.packageName == "com.roblox.RobloxStudio") {
            LOG_INFO("Found Roblox: " + app.packageName);
            return true;
        }
    }
    
    LOG_WARNING("Roblox not found in running apps");
    return false;
}

OptimizationResult AndroidOptimizer::optimizeProcessPriority() {
    // On Android, we can't directly change process priority without root
    // But we can request performance mode
    if (SystemManager::enablePerformanceMode()) {
