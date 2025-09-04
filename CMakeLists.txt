# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(RobloxOptimizer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform detection
if(ANDROID)
    set(ANDROID_BUILD ON)
    message(STATUS "Building for Android")
elseif(WIN32)
    set(WINDOWS_BUILD ON)
    message(STATUS "Building for Windows")
elseif(UNIX)
    set(LINUX_BUILD ON)
    message(STATUS "Building for Linux")
endif()

# Source files
set(COMMON_SOURCES
    src/common/Logger.cpp
    src/common/Utils.cpp
    src/common/Config.cpp
)

set(WINDOWS_SOURCES
    src/windows/WindowsOptimizer.cpp
    src/windows/ProcessManager.cpp
    src/windows/RegistryManager.cpp
    src/windows/main_windows.cpp
)

set(ANDROID_SOURCES
    src/android/AndroidOptimizer.cpp
    src/android/SystemManager.cpp
    src/android/main_android.cpp
)

set(LINUX_SOURCES
    src/linux/LinuxOptimizer.cpp
    src/linux/ProcessManager.cpp
    src/linux/main_linux.cpp
)

# Include directories
include_directories(
    include/common
    include/platform
)

# Windows build
if(WINDOWS_BUILD)
    add_executable(RobloxOptimizer 
        ${COMMON_SOURCES}
        ${WINDOWS_SOURCES}
    )
    
    target_link_libraries(RobloxOptimizer 
        psapi
        advapi32
        kernel32
        user32
    )
    
    target_include_directories(RobloxOptimizer PRIVATE 
        include/windows
    )
    
    # Windows specific definitions
    target_compile_definitions(RobloxOptimizer PRIVATE 
        WINDOWS_BUILD=1
        UNICODE=1
        _UNICODE=1
    )

# Android build
elseif(ANDROID_BUILD)
    add_library(RobloxOptimizerAndroid SHARED
        ${COMMON_SOURCES}
        ${ANDROID_SOURCES}
    )
    
    target_link_libraries(RobloxOptimizerAndroid
        log
        android
    )
    
    target_include_directories(RobloxOptimizerAndroid PRIVATE 
        include/android
    )
    
    target_compile_definitions(RobloxOptimizerAndroid PRIVATE 
        ANDROID_BUILD=1
    )

# Linux build (for testing/development)
elseif(LINUX_BUILD)
    add_executable(RobloxOptimizerLinux
        ${COMMON_SOURCES}
        ${LINUX_SOURCES}
    )
    
    target_link_libraries(RobloxOptimizerLinux
        pthread
    )
    
    target_include_directories(RobloxOptimizerLinux PRIVATE 
        include/linux
    )
    
    target_compile_definitions(RobloxOptimizerLinux PRIVATE 
        LINUX_BUILD=1
    )
endif()

# Compiler-specific options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Build type specific settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG=1)
endif()
