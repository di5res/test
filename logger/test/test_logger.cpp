#include <iostream>
#include <thread>
#include "../include/Logger.hpp"

void testThread(int threadId) {
    for (int i = 0; i < 5; ++i) {
        Logger::getInstance().log(LogLevel::Info, 
            "Thread " + std::to_string(threadId) + " message " + std::to_string(i));
    }
}

int main() {
    try {
        // Инициализация логгера
        Logger::getInstance().init("application.log", LogLevel::Debug);
        
        // Логирование сообщений разных уровней
        Logger::getInstance().log(LogLevel::Debug, "This is a debug message");
        Logger::getInstance().log(LogLevel::Info, "Application started");
        Logger::getInstance().log(LogLevel::Warning, "Low memory detected");
        Logger::getInstance().log(LogLevel::Error, "Failed to open file");
        Logger::getInstance().log(LogLevel::Critical, "Critical system error");
        
        // Меняем уровень логирования
        Logger::getInstance().setLevel(LogLevel::Warning);
        Logger::getInstance().log(LogLevel::Info, "This message won't be logged");
        
        // Тест многопоточности
        std::thread t1(testThread, 1);
        std::thread t2(testThread, 2);
        t1.join();
        t2.join();
        
        // Тест обработки ошибок
        try {
            Logger::getInstance().log(LogLevel::Info, std::string(1000000, 'x')); // Очень большое сообщение
        } catch (const std::exception& e) {
            std::cerr << "Error caught: " << e.what() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Logger initialization failed: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
