// include/android/AndroidOptimizer.h
#pragma once
#ifdef ANDROID_BUILD

#include "BaseOptimizer.h"
#include <jni.h>

class AndroidOptimizer : public BaseOptimizer {
private:
    JavaVM* jvm;
    jobject activityObject;
    std::string packageName;
    
public:
    AndroidOptimizer();
    ~AndroidOptimizer() override;
    
    // Inherited from BaseOptimizer  
    bool findRobloxProcess() override;
    OptimizationResult optimizeProcessPriority() override;
    OptimizationResult optimizeMemory() override;
    OptimizationResult optimizeSystemSettings() override;
    ProcessInfo getProcessInfo() override;
    
    // Android-specific methods
    bool setJavaVM(JavaVM* vm);
    bool setActivityObject(jobject activity);
    OptimizationResult optimizeCpuGovernor();
    OptimizationResult optimizeGpuFrequency();
    OptimizationResult clearCache();
    OptimizationResult optimizeBatterySettings();
    
private:
    JNIEnv* getJNIEnv();
    bool callJavaMethod(const char* methodName, const char* signature, ...);
};
