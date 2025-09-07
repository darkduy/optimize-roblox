#!/bin/bash
# build-scripts/build.sh - Build script for supported Roblox platforms only

set -e

# Build script for Roblox-supported platforms only
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
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

log_platform() {
    echo -e "${CYAN}[PLATFORM]${NC} $1"
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
            echo ""
            echo "🎮 Roblox System Optimizer Build Script"
            echo "   Supports only platforms where Roblox runs officially"
            echo ""
            echo "Options:"
            echo "  -p, --platform PLATFORM    Target platform (windows|android|auto)"
            echo "  -t, --type TYPE            Build type (Release|Debug)"
            echo "  -c, --clean                Clean build directory before building"
            echo "  -h, --help                 Show this help message"
            echo ""
            echo "✅ Supported Platforms:"
            echo "  • windows    - Windows 10/11 x64 only"
            echo "  • android    - Android 8.0+ (API 26+)"
            echo ""
            echo "❌ Unsupported Platforms:"
            echo "  • Windows 7/8/8.1 (Roblox discontinued)"
            echo "  • 32-bit Windows (Roblox discontinued)"  
            echo "  • Android 7.x and below (performance limitations)"
            echo "  • Linux (Roblox not available)"
            echo ""
            echo "Examples:"
            echo "  $0 --platform windows --type Release"
            echo "  $0 --platform android --clean"
            echo "  $0  # Auto-detect platform"
            exit 0
            ;;
        *)
            log_error "Unknown option: $1"
            log_info "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Platform validation and auto-detection
validate_platform() {
    case "$1" in
        windows|android)
            return 0
            ;;
        *)
            return 1
            ;;
    esac
}

# Auto-detect platform if not specified
if [ "$PLATFORM" = "auto" ]; then
    if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
        PLATFORM="windows"
        log_info "Auto-detected: Windows"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        if [ -n "$ANDROID_NDK_HOME" ] || [ -n "$ANDROID_HOME" ]; then
            PLATFORM="android"
            log_info "Auto-detected: Android (NDK environment found)"
        else
            log_error "Linux detected but this is not supported for Roblox optimization"
            log_warn "Roblox is not available on Linux"
            log_info "Supported platforms: Windows 10/11 x64, Android 8.0+"
            exit 1
        fi
    else
        log_error "Cannot auto-detect supported platform"
        log_warn "Current OS: $OSTYPE"
        log_info "Please specify platform manually: --platform windows|android"
        exit 1
    fi
fi

# Validate platform
if ! validate_platform "$PLATFORM"; then
    log_error "Unsupported platform: $PLATFORM"
    log_warn "This tool only supports platforms where Roblox runs:"
    log_info "• Windows 10/11 (x64 only)"
    log_info "• Android 8.0+ (API 26+)"
    exit 1
fi

# Platform compatibility checks
check_windows_compatibility() {
    if ! command -v cmake >/dev/null 2>&1; then
        log_error "CMake not found. Please install CMake 3.16+"
        exit 1
    fi
    
    # Check if we're on a supported Windows version
    if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
        # Check Windows version (basic check)
        if command -v systeminfo >/dev/null 2>&1; then
            windows_version=$(systeminfo | grep "OS Name" || echo "Unknown")
            log_info "Detected: $windows_version"
        fi
    fi
    
    log_platform "✅ Building for Windows 10/11 x64 (Roblox compatible)"
}

check_android_compatibility() {
    if [ -z "$ANDROID_NDK_HOME" ]; then
        log_error "ANDROID_NDK_HOME not set"
        log_info "Please set Android NDK path for Android 8.0+ builds"
        exit 1
    fi
    
    if [ ! -d "$ANDROID_NDK_HOME" ]; then
        log_error "Android NDK directory not found: $ANDROID_NDK_HOME"
        exit 1
    fi
    
    log_platform "✅ Building for Android 8.0+ (API 26+, Roblox compatible)"
}

# Header
echo -e "${BLUE}╔═══════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║              🎮 ROBLOX OPTIMIZER BUILD                    ║${NC}"  
echo -e "${BLUE}║          Supports only Roblox-compatible platforms       ║${NC}"
echo -e "${BLUE}╚═══════════════════════════════════════════════════════════╝${NC}"
echo ""

log_step "🔍 Configuration"
log_info "Platform: $PLATFORM"
log_info "Build type: $BUILD_TYPE"
log_info "Clean build: $CLEAN_BUILD"
echo ""

cd "$PROJECT_ROOT"

# Clean build if requested
if [ "$CLEAN_BUILD" = true ]; then
    log_step "🧹 Cleaning build directories..."
    rm -rf build build-android-* packages artifacts
    log_info "Build directories cleaned"
fi

# Build based on platform
case $PLATFORM in
    windows)
        log_step "🖥️  Building for Windows 10/11..."
        check_windows_compatibility
        
        log_info "Configuring CMake for Windows x64..."
        cmake -B build \
            -G "Visual Studio 17 2022" \
            -A x64 \
            -DWINDOWS_BUILD=ON \
            -DTARGET_WINDOWS_10_PLUS=ON \
            -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
        
        log_info "Building Windows x64 executable..."
        cmake --build build --config "$BUILD_TYPE" --parallel $(nproc 2>/dev/null || echo 4)
        
        # Check build result
        if [ -f "build/$BUILD_TYPE/RobloxOptimizer.exe" ]; then
            log_info "✅ Windows build successful!"
            log_info "📁 Output: build/$BUILD_TYPE/RobloxOptimizer.exe"
        else
            log_error "❌ Windows build failed - executable not found"
            exit 1
        fi
        ;;
        
    android)
        log_step "📱 Building for Android 8.0+..."
        check_android_compatibility
        
        # Build for modern Android architectures only
        ANDROID_ABIS=("arm64-v8a" "x86_64")
        
        for ABI in "${ANDROID_ABIS[@]}"; do
            log_info "Building for ABI: $ABI (Android 8.0+ compatible)"
            
            cmake -B "build-android-$ABI" \
                -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake" \
                -DANDROID_ABI="$ABI" \
                -DANDROID_NATIVE_API_LEVEL=26 \
                -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
                -DANDROID_BUILD=ON \
                -DTARGET_ANDROID_8_PLUS=ON \
                -DCMAKE_ANDROID_STL_TYPE=c++_shared
            
            cmake --build "build-android-$ABI" --config "$BUILD_TYPE" --parallel $(nproc 2>/dev/null || echo 4)
            
            # Check build result  
            if [ -f "build-android-$ABI/libRobloxOptimizerAndroid.so" ]; then
                log_info "✅ Android $ABI build successful!"
            else
                log_warn "⚠️  Android $ABI build may have issues"
            fi
        done
        
        log_info "📁 Android outputs in: build-android-*/"
        ;;
        
    *)
        log_error "This should never happen - platform validation failed"
        exit 1
        ;;
esac

# Build summary
echo ""
log_step "🎉 Build completed successfully!"
echo ""
log_platform "Platform Summary:"
case $PLATFORM in
    windows)
        echo -e "  🖥️  ${GREEN}Windows 10/11 x64${NC} - Roblox compatible"
        echo -e "  📋 Features: Process optimization, registry tweaks, monitoring"
        echo -e "  🎯 Target: Modern Windows systems only"
        ;;
    android)  
        echo -e "  📱 ${GREEN}Android 8.0+ (API 26+)${NC} - Roblox compatible"
        echo -e "  📋 Features: CPU/GPU optimization, system tweaks" 
        echo -e "  🎯 Target: Modern Android devices only"
        ;;
esac

echo ""
log_info "🎮 Ready to optimize Roblox performance!"
log_warn "⚠️  Remember: Only use on supported Roblox platforms"
echo ""

# Next steps
case $PLATFORM in
    windows)
        echo -e "${CYAN}Next steps:${NC}"
        echo "1. Run as Administrator: build/$BUILD_TYPE/RobloxOptimizer.exe"
        echo "2. Open Roblox before running optimizer"
        echo "3. Select 'Auto Optimize' option"
        echo "4. Enjoy improved performance! 🚀"
        ;;
    android)
        echo -e "${CYAN}Next steps:${NC}"
        echo "1. Install APK on Android 8.0+ device"
        echo "2. Grant necessary permissions"
        echo "3. Run optimizer before launching Roblox"
        echo "4. Tap 'Optimize' and enjoy smooth gameplay! 📱"
        ;;
esac

echo ""
log_info "📖 See README.md for detailed usage instructions"
log_info "🐛 Report issues at: https://github.com/darkduy/optimize-roblox/issues"
echo ""

# Warning for unsupported platforms
echo -e "${YELLOW}⚠️  Platform Compatibility Warning:${NC}"
echo "   This optimizer ONLY works on platforms where Roblox runs:"
echo "   • Windows 10 version 1903+ or Windows 11 (x64 only)"
echo "   • Android 8.0 Oreo+ (API 26+, ARM64/x86_64)"
echo ""
echo "   Unsupported platforms:"
echo "   ❌ Windows 7/8/8.1 (Roblox discontinued support)"
echo "   ❌ 32-bit Windows (Roblox requires 64-bit)"
echo "   ❌ Android 7.x and below (performance limitations)"
echo "   ❌ Linux/macOS (Roblox not available)"

exit 0
