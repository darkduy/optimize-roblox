# build-scripts/build.sh
#!/bin/bash

set -e

# Build script for multiple platforms
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
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

# Default values
BUILD_TYPE="Release"
PLATFORM="auto"
CLEAN_BUILD=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--platform)
            PLATFORM="$2"
            shift 2
            ;;
        -t|--type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  -p, --platform PLATFORM    Target platform (windows|android|linux|auto)"
            echo "  -t, --type TYPE            Build type (Release|Debug)"
            echo "  -c, --clean                Clean build directory before building"
            echo "  -h, --help                 Show this help message"
            exit 0
            ;;
        *)
            log_error "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Auto-detect platform if not specified
if [ "$PLATFORM" = "auto" ]; then
    if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]; then
        PLATFORM="windows"
    elif [[ "$OSTYPE" == "linux-android"* ]]; then
        PLATFORM="android"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        PLATFORM="linux"
    else
        log_error "Cannot auto-detect platform. Please specify with -p flag."
        exit 1
    fi
fi

log_info "Building for platform: $PLATFORM"
log_info "Build type: $BUILD_TYPE"

cd "$PROJECT_ROOT"

# Clean build if requested
if [ "$CLEAN_BUILD" = true ]; then
    log_info "Cleaning build directories..."
    rm -rf build build-android build-linux
fi

# Build based on platform
case $PLATFORM in
    windows)
        log_info "Building for Windows..."
        if command -v cmake >/dev/null 2>&1; then
            cmake -B build -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
            cmake --build build --config "$BUILD_TYPE"
        else
            log_error "CMake not found. Please install CMake."
            exit 1
        fi
        ;;
    
    android)
        log_info "Building for Android..."
        if [ -z "$ANDROID_NDK_HOME" ]; then
            log_error "ANDROID_NDK_HOME not set. Please set Android NDK path."
            exit 1
        fi
        
        for ABI in "armeabi-v7a" "arm64-v8a" "x86" "x86_64"; do
            log_info "Building for ABI: $ABI"
            cmake -B "build-android-$ABI" \
                -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake" \
                -DANDROID_ABI="$ABI" \
                -DANDROID_NATIVE_API_LEVEL=21 \
                -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
                -DANDROID_BUILD=ON
            cmake --build "build-android-$ABI" --config "$BUILD_TYPE"
        done
        ;;
    
    linux)
        log_info "Building for Linux..."
        cmake -B build-linux -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DLINUX_BUILD=ON
        cmake --build build-linux
        ;;
    
    *)
        log_error "Unknown platform: $PLATFORM"
        exit 1
        ;;
esac

log_info "Build completed successfully!"
