# build-scripts/package.sh
#!/bin/bash

# Packaging script
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
PACKAGE_DIR="$PROJECT_ROOT/packages"

# Create package directory
mkdir -p "$PACKAGE_DIR"

# Package Windows builds
if [ -d "$PROJECT_ROOT/build" ]; then
    echo "Packaging Windows builds..."
    cd "$PROJECT_ROOT"
    
    for config in Release Debug; do
        if [ -f "build/$config/RobloxOptimizer.exe" ]; then
            package_name="RobloxOptimizer-Windows-$config"
            mkdir -p "$PACKAGE_DIR/$package_name"
            
            cp "build/$config/RobloxOptimizer.exe" "$PACKAGE_DIR/$package_name/"
            cp README.md "$PACKAGE_DIR/$package_name/"
            cp LICENSE "$PACKAGE_DIR/$package_name/" 2>/dev/null || true
            
            cd "$PACKAGE_DIR"
            zip -r "${package_name}.zip" "$package_name"
            rm -rf "$package_name"
            cd "$PROJECT_ROOT"
            
            echo "Created: $PACKAGE_DIR/${package_name}.zip"
        fi
    done
fi

# Package Android builds
for ABI in armeabi-v7a arm64-v8a x86 x86_64; do
    if [ -d "$PROJECT_ROOT/build-android-$ABI" ]; then
        echo "Packaging Android build for $ABI..."
        cd "$PROJECT_ROOT"
        
        package_name="RobloxOptimizer-Android-$ABI"
        mkdir -p "$PACKAGE_DIR/$package_name"
        
        cp "build-android-$ABI/libRobloxOptimizerAndroid.so" "$PACKAGE_DIR/$package_name/" 2>/dev/null || true
        cp README.md "$PACKAGE_DIR/$package_name/"
        cp LICENSE "$PACKAGE_DIR/$package_name/" 2>/dev/null || true
        
        cd "$PACKAGE_DIR"
        tar -czf "${package_name}.tar.gz" "$package_name"
        rm -rf "$package_name"
        cd "$PROJECT_ROOT"
        
        echo "Created: $PACKAGE_DIR/${package_name}.tar.gz"
    fi
done

echo "Packaging completed!"
