#!/bin/bash
# setup-project.sh - Tạo cấu trúc project Roblox Optimizer

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

log_step() {
    echo -e "${BLUE}[STEP]${NC} $1"
}

log_step "🚀 Setting up Roblox Optimizer project structure..."

# Create directory structure
log_info "Creating directory structure..."
mkdir -p {.github/workflows,build-scripts,src/{common,windows,android,linux},include/{common,windows,android,linux}}

# Create .gitignore
log_info "Creating .gitignore..."
cat > .gitignore << 'EOF'
# Build artifacts
*.exe
*.so
*.dll
*.dylib
*.o
*.obj
*.pdb
*.ilk
*.exp
*.lib
build/
build-*/
packages/
artifacts/

# IDE files
.vs/
.vscode/
*.user
*.tmp
*.log
CMakeCache.txt
CMakeFiles/
cmake_install.cmake
Makefile
*.cbp
*.sln
*.vcxproj*
*.xcodeproj/
*.xcworkspace/

# Android specific
*.apk
*.aab
*.dex
.gradle/
local.properties

# macOS
.DS_Store

# Windows
Thumbs.db
EOF

# Create basic CMakeLists.txt
log_info "Creating CMakeLists.txt..."
cat > CMakeLists.txt << 'EOF'
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
