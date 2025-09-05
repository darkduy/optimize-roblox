// src/windows/main_windows.cpp
#ifdef WINDOWS_BUILD
#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "WindowsOptimizer.h"
#include "Logger.h"
#include "Config.h"
#include "Utils.h"

void printHeader() {
    system("cls");
    std::wcout << L"╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::wcout << L"║                 ROBLOX SYSTEM OPTIMIZER v1.0                ║" << std::endl;
    std::wcout << L"║                    Windows Edition                           ║" << std::endl;
    std::wcout << L"╠══════════════════════════════════════════════════════════════╣" << std::endl;
    std::wcout << L"║ Tối ưu hóa hiệu năng Roblox một cách an toàn                ║" << std::endl;
    std::wcout << L"║ Không chỉnh sửa game files - Hoàn toàn hợp pháp!           ║" << std::endl;
    std::wcout << L"╚══════════════════════════════════════════════════════════════╝" << std::endl;
    std::wcout << std::endl;
}

void printMenu() {
    std::wcout << L"┌─────────────────────────────────────────────────────────────┐" << std::endl;
    std::wcout << L"│                        MENU CHÍNH                           │" << std::endl;
    std::wcout << L"├─────────────────────────────────────────────────────────────┤" << std::endl;
    std::wcout << L"│ 1. Tự động tối ưu Roblox                                   │" << std::endl;
    std::wcout << L"│ 2. Tối ưu hệ thống Windows                                 │" << std::endl;
    std::wcout << L"│ 3. Theo dõi hiệu năng realtime                            │" << std::endl;
    std::wcout << L"│ 4. Cài đặt cấu hình                                       │" << std::endl;
    std::wcout << L"│ 5. Xem log hệ thống                                       │" << std::endl;
    std::wcout << L"│ 0. Thoát                                                   │" << std::endl;
    std::wcout << L"└─────────────────────────────────────────────────────────────┘" << std::endl;
    std::wcout << L"Chọn: ";
}

void autoOptimize() {
    WindowsOptimizer optimizer;
    
    std::wcout << L"Đang tìm kiếm Roblox..." << std::endl;
    
    if (!optimizer.findRobloxProcess()) {
        std::wcout << L"❌ Không tìm thấy Roblox đang chạy!" << std::endl;
        std::wcout << L"   Vui lòng mở Roblox trước khi chạy optimizer." << std::endl;
        system("pause");
        return;
    }
    
    std::wcout << L"✅ Đã tìm thấy Roblox!" << std::endl;
    std::wcout << L"Đang áp dụng tối ưu hóa..." << std::endl;
    
    // Process optimization
    auto result = optimizer.optimizeProcessPriority();
    if (result.success) {
        std::wcout << L"✅ " << std::wstring(result.message.begin(), result.message.end()) << std::endl;
    } else {
        std::wcout << L"❌ " << std::wstring(result.message.begin(), result.message.end()) << std::endl;
    }
    
    // Memory optimization
    result = optimizer.optimizeMemory();
    if (result.success) {
        std::wcout << L"✅ " << std::wstring(result.message.begin(), result.message.end()) << std::endl;
    } else {
        std::wcout << L"❌ " << std::wstring(result.message.begin(), result.message.end()) << std::endl;
    }
    
    // System optimization
    result = optimizer.optimizeSystemSettings();
    std::wcout << L"📊 Kết quả tối ưu hệ thống:" << std::endl;
    std::wcout << std::wstring(result.details.begin(), result.details.end()) << std::endl;
    
    std::wcout << L"🚀 Tối ưu hóa hoàn tất! Roblox của bạn sẽ chạy mượt hơn." << std::endl;
    system("pause");
}

void monitorPerformance() {
    WindowsOptimizer optimizer;
    
    if (!optimizer.findRobloxProcess()) {
        std::wcout << L"❌ Không tìm thấy Roblox đang chạy!" << std::endl;
        system("pause");
        return;
    }
    
    std::wcout << L"📊 Chế độ theo dõi hiệu năng - Nhấn ESC để dừng" << std::endl;
    
    while (true) {
        if (_kbhit() && _getch() == 27) { // ESC key
            break;
        }
        
        system("cls");
        printHeader();
        
        auto info = optimizer.getProcessInfo();
        
        std::wcout << L"┌─────────────────────────────────────────────────────────────┐" << std::endl;
        std::wcout << L"│                    THÔNG TIN HIỆU NĂNG                     │" << std::endl;
        std::wcout << L"├─────────────────────────────────────────────────────────────┤" << std::endl;
        std::wcout << L"│ Process ID: " << info.pid << std::endl;
        std::wcout << L"│ Tên process: " << std::wstring(info.name.begin(), info.name.end()) << std::endl;
        std::wcout << L"│ Memory Usage: " << std::wstring(Utils::formatBytes(info.memoryUsage).begin(), 
                                                      Utils::formatBytes(info.memoryUsage).end()) << std::endl;
        std::wcout << L"│ CPU Usage: " << std::fixed << std::setprecision(1) << info.cpuUsage << L"%" << std::endl;
        std::wcout << L"│ Trạng thái: " << (info.isRunning ? L"🟢 Đang chạy" : L"🔴 Đã dừng") << std::endl;
        std::wcout << L"└─────────────────────────────────────────────────────────────┘" << std::endl;
        std::wcout << L"Nhấn ESC để thoát..." << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main() {
    // Set console UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // Initialize logger
    Logger::getInstance()->setLevel(LogLevel::INFO);
    LOG_INFO("Roblox Optimizer started");
    
    // Load config
    Config* config = Config::getInstance();
    
    int choice;
    
    while (true) {
        printHeader();
        printMenu();
        
        std::wcin >> choice;
        
        switch (choice) {
            case 1:
                autoOptimize();
                break;
                
            case 2: {
                WindowsOptimizer optimizer;
                auto result = optimizer.optimizeSystemSettings();
                std::wcout << std::wstring(result.message.begin(), result.message.end()) << std::endl;
                std::wcout << std::wstring(result.details.begin(), result.details.end()) << std::endl;
                system("pause");
                break;
            }
            
            case 3:
                monitorPerformance();
                break;
                
            case 4:
                std::wcout << L"⚙️ Cài đặt cấu hình (Tính năng sẽ được bổ sung)" << std::endl;
                system("pause");
                break;
                
            case 5:
                std::wcout << L"📋 Xem log hệ thống (Tính năng sẽ được bổ sung)" << std::endl;
                system("pause");
                break;
                
            case 0:
                std::wcout << L"👋 Tạm biệt!" << std::endl;
                LOG_INFO("Roblox Optimizer stopped");
                return 0;
                
            default:
                std::wcout << L"❌ Lựa chọn không hợp lệ!" << std::endl;
                system("pause");
                break;
        }
    }
    
    return 0;
}

#endif // WINDOWS_BUILD
