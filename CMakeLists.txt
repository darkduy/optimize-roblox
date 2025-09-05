# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(RobloxOptimizer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform detection and validation
if(ANDROID)
    set(ANDROID_BUILD ON)
    message(STATUS "Building for Android 8.0+ (API 26+)")
    
    # Validate Android API level
    if(ANDROID_NATIVE_API_LEVEL LESS 26)
        message(FATAL_ERROR "Android API Level 26+ required (Android 8.0+). Current: ${ANDROID_NATIVE_API_LEVEL}")
    endif()
    
elseif(WIN32)
    set(WINDOWS_BUILD ON)
    message(STATUS "Building for Windows 10/11 (x64 only)")
    
    # Ensure we're building for x64 only
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        message(FATAL_ERROR "32-bit Windows builds not supported. Roblox requires 64-bit Windows.")
    endif()
    
    # Set Windows 10 as minimum target
    add_compile_definitions(_WIN32_WINNT=0x0A00)  # Windows 10
    
else()
    message(FATAL_ERROR "Unsupported platform. Only Windows 10/11 (x64) and Android 8.0+ are supported.")
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common
    ${CMAKE_CURRENT_SOURCE_DIR}/include/platform
)

# Source files
set(COMMON_SOURCES
    src/common/Logger.cpp
    src/common/Utils.cpp
    src/common/Config.cpp
)

# Create directories if they don't exist
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/common)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/common)

# Check if source files exist, create empty ones if not
set(REQUIRED_FILES
    "src/common/Logger.cpp"
    "src/common/Utils.cpp" 
    "src/common/Config.cpp"
)

# Windows build (Windows 10/11 x64 only)
if(WINDOWS_BUILD)
    message(STATUS "Configuring Windows build for modern systems...")
    
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/windows)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/windows)
    
    list(APPEND REQUIRED_FILES
        "src/windows/WindowsOptimizer.cpp"
        "src/windows/ProcessManager.cpp"
        "src/windows/RegistryManager.cpp"
        "src/windows/main_windows.cpp"
    )
    
    # Create files if they don't exist
    foreach(file ${REQUIRED_FILES})
        if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${file})
            if(${file} MATCHES "main_windows.cpp")
                file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/${file} 
                    "// Windows main - Windows 10/11 x64 only\n#ifdef WINDOWS_BUILD\n#include <iostream>\nint main() {\n    std::cout << \"Roblox Optimizer for Windows 10/11 x64\" << std::endl;\n    return 0;\n}\n#endif\n")
            else()
                file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/${file} 
                    "// Windows implementation - Windows 10/11 x64 only\n#ifdef WINDOWS_BUILD\n// Implementation here\n#endif\n")
            endif()
        endif()
    endforeach()
    
    set(WINDOWS_SOURCES
        src/windows/WindowsOptimizer.cpp
        src/windows/ProcessManager.cpp
        src/windows/RegistryManager.cpp
        src/windows/main_windows.cpp
    )
    
    add_executable(RobloxOptimizer 
        ${COMMON_SOURCES}
        ${WINDOWS_SOURCES}
    )
    
    # Windows 10/11 specific libraries
    target_link_libraries(RobloxOptimizer 
        psapi
        advapi32
        kernel32
        user32
        version
        winmm
        dwmapi  # For modern Windows features
    )
    
    target_include_directories(RobloxOptimizer PRIVATE 
        include/windows
    )
    
    target_compile_definitions(RobloxOptimizer PRIVATE 
        WINDOWS_BUILD=1
        TARGET_WINDOWS_10_PLUS=1
        UNICODE=1
        _UNICODE=1
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
    
    # Set Windows 10 compatibility
    set_property(TARGET RobloxOptimizer PROPERTY WIN32_EXECUTABLE TRUE)

# Android build (Android 8.0+ API 26+)
elseif(ANDROID_BUILD)
    message(STATUS "Configuring Android build for modern devices...")
    
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/android)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/android)
    
    list(APPEND REQUIRED_FILES
        "src/android/AndroidOptimizer.cpp"
        "src/android/SystemManager.cpp"
        "src/android/main_android.cpp"
    )
    
    # Create files if they don't exist
    foreach(file ${REQUIRED_FILES})
        if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${file})
            file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/${file} 
                "// Android implementation - Android 8.0+ (API 26+)\n#ifdef ANDROID_BUILD\n// Implementation here\n#endif\n")
        endif()
    endforeach()
    
    set(ANDROID_SOURCES
        src/android/AndroidOptimizer.cpp
        src/android/SystemManager.cpp
        src/android/main_android.cpp
    )
    
    add_library(RobloxOptimizerAndroid SHARED
        ${COMMON_SOURCES}
        ${ANDROID_SOURCES}
    )
    
    # Find Android libraries
    find_library(log-lib log)
    find_library(android-lib android)
    
    target_link_libraries(RobloxOptimizerAndroid
        ${log-lib}
        ${android-lib}
    )
    
    target_include_directories(RobloxOptimizerAndroid PRIVATE 
        include/android
    )
    
    target_compile_definitions(RobloxOptimizerAndroid PRIVATE 
        ANDROID_BUILD=1
        TARGET_ANDROID_8_PLUS=1
        __ANDROID_API__=${ANDROID_NATIVE_API_LEVEL}
    )
    
    # Android 8.0+ specific features
    if(ANDROID_NATIVE_API_LEVEL GREATER_EQUAL 26)
        target_compile_definitions(RobloxOptimizerAndroid PRIVATE ANDROID_8_FEATURES=1)
    endif()
    
    if(ANDROID_NATIVE_API_LEVEL GREATER_EQUAL 28)
