
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
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/windows)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/android)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/linux)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/common)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/windows)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/android)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/linux)

# Check if source files exist, create empty ones if not
set(REQUIRED_FILES
    "src/common/Logger.cpp"
    "src/common/Utils.cpp" 
    "src/common/Config.cpp"
)

# Windows build
if(WINDOWS_BUILD)
    list(APPEND REQUIRED_FILES
        "src/windows/WindowsOptimizer.cpp"
        "src/windows/ProcessManager.cpp"
        "src/windows/RegistryManager.cpp"
        "src/windows/main_windows.cpp"
    )
    
    # Create files if they don't exist
    foreach(file ${REQUIRED_FILES})
        if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${file})
            file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/${file} 
                "// Placeholder file - implement functionality here\n#ifdef WINDOWS_BUILD\n// Windows implementation\n#endif\n")
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
    
    target_link_libraries(RobloxOptimizer 
        psapi
        advapi32
        kernel32
        user32
    )
    
    target_include_directories(RobloxOptimizer PRIVATE 
        include/windows
    )
    
    target_compile_definitions(RobloxOptimizer PRIVATE 
        WINDOWS_BUILD=1
        UNICODE=1
        _UNICODE=1
    )

# Android build
elseif(ANDROID_BUILD)
    list(APPEND REQUIRED_FILES
        "src/android/AndroidOptimizer.cpp"
        "src/android/SystemManager.cpp"
        "src/android/main_android.cpp"
    )
    
    # Create files if they don't exist
    foreach(file ${REQUIRED_FILES})
        if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${file})
            file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/${file} 
                "// Placeholder file - implement functionality here\n#ifdef ANDROID_BUILD\n// Android implementation\n#endif\n")
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
    )

# Linux build (for testing/development)
elseif(LINUX_BUILD)
    list(APPEND REQUIRED_FILES
        "src/linux/LinuxOptimizer.cpp"
        "src/linux/ProcessManager.cpp"
        "src/linux/main_linux.cpp"
    )
    
    # Create files if they don't exist
    foreach(file ${REQUIRED_FILES})
        if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${file})
            file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/${file} 
                "// Placeholder file - implement functionality here\n#ifdef LINUX_BUILD\n// Linux implementation\nint main() { return 0; }\n#endif\n")
        endif()
    endforeach()
    
    set(LINUX_SOURCES
        src/linux/LinuxOptimizer.cpp
        src/linux/ProcessManager.cpp
        src/linux/main_linux.cpp
    )
    
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

# Create header files if they don't exist
set(HEADER_FILES
    "include/common/Logger.h"
    "include/common/Utils.h"
    "include/common/Config.h"
    "include/common/BaseOptimizer.h"
)

if(WINDOWS_BUILD)
    list(APPEND HEADER_FILES
        "include/windows/WindowsOptimizer.h"
        "include/windows/ProcessManager.h"
        "include/windows/RegistryManager.h"
    )
elseif(ANDROID_BUILD)
    list(APPEND HEADER_FILES
        "include/android/AndroidOptimizer.h"
        "include/android/SystemManager.h"
    )
endif()

foreach(header ${HEADER_FILES})
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${header})
        file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/${header} 
            "#pragma once\n// Header file - define interfaces here\n")
    endif()
endforeach()

# Compiler-specific options
if(MSVC)
    if(TARGET RobloxOptimizer)
        target_compile_options(RobloxOptimizer PRIVATE /W4)
    endif()
else()
    set(targets)
    if(TARGET RobloxOptimizer)
        list(APPEND targets RobloxOptimizer)
    endif()
    if(TARGET RobloxOptimizerAndroid)
        list(APPEND targets RobloxOptimizerAndroid)
    endif()
    if(TARGET RobloxOptimizerLinux)
        list(APPEND targets RobloxOptimizerLinux)
    endif()
    
    foreach(target ${targets})
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endforeach()
endif()

# Build type specific settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(TARGET RobloxOptimizer)
        target_compile_definitions(RobloxOptimizer PRIVATE DEBUG=1)
    endif()
    if(TARGET RobloxOptimizerAndroid)
        target_compile_definitions(RobloxOptimizerAndroid PRIVATE DEBUG=1)
    endif()
    if(TARGET RobloxOptimizerLinux)
        target_compile_definitions(RobloxOptimizerLinux PRIVATE DEBUG=1)
    endif()
else()
    if(TARGET RobloxOptimizer)
        target_compile_definitions(RobloxOptimizer PRIVATE NDEBUG=1)
    endif()
    if(TARGET RobloxOptimizerAndroid)
        target_compile_definitions(RobloxOptimizerAndroid PRIVATE NDEBUG=1)
    endif()
    if(TARGET RobloxOptimizerLinux)
        target_compile_definitions(RobloxOptimizerLinux PRIVATE NDEBUG=1)
    endif()
endif()
