#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include <memory>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>

enum class LogLevel {
    Debug,      // Отладочные сообщения
    Info,       // Информационные сообщения
    Warning,    // Предупреждения
    Error,      // Ошибки
    Critical    // Критические ошибки
};

class Logger {
public:
    // Удаляем конструктор копирования и оператор присваивания
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Получение экземпляра логгера (синглтон)
    static Logger& getInstance();

    // Инициализация логгера
    void init(const std::string& filename, LogLevel level = LogLevel::Info);
    
    // Установка уровня логирования
    void setLevel(LogLevel level);
    
    // Запись сообщения в лог
    void log(LogLevel level, const std::string& message);
    
    // Проверка инициализации
    bool isInitialized() const;

private:
    Logger() = default;
    ~Logger();

    std::string levelToString(LogLevel level) const;
    std::string getCurrentTime() const;
    void checkFileState();

    std::ofstream logFile_;
    LogLevel currentLevel_ = LogLevel::Info;
    bool initialized_ = false;
    mutable std::mutex mutex_;
};

#endif // LOGGER_HPP