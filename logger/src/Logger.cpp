#include "../include/Logger.hpp"
#include <stdexcept>
#include <system_error>

Logger::~Logger() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::init(const std::string& filename, LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (initialized_) {
        logFile_.close();
    }

    logFile_.open(filename, std::ios::app);
    if (!logFile_.is_open()) {
        throw std::system_error(errno, std::system_category(), 
                              "Failed to open log file: " + filename);
    }

    currentLevel_ = level;
    initialized_ = true;
    
    // Записываем сообщение о старте логгера напрямую, чтобы избежать рекурсивной блокировки мьютекса
    logFile_ << "[" << getCurrentTime() << "] "
             << "[" << levelToString(LogLevel::Info) << "] "
             << "Logger initialized" << std::endl;
}

void Logger::setLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    currentLevel_ = level;
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_); // Блокировка в начале для потокобезопасного доступа к initialized_
    if (!initialized_ || level < currentLevel_) {
        return;
    }
    checkFileState();

    logFile_ << "[" << getCurrentTime() << "] "
             << "[" << levelToString(level) << "] "
             << message << std::endl;
    
    // Проверяем, не произошла ли ошибка при записи
    if (logFile_.fail()) {
        throw std::runtime_error("Failed to write to log file");
    }
}

bool Logger::isInitialized() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return initialized_;
}

std::string Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO";
        case LogLevel::Warning:  return "WARNING";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}

std::string Logger::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm bt{};
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__)) // Для GCC/Clang на Unix-подобных системах
    localtime_r(&in_time_t, &bt);
#elif defined(_MSC_VER) // Для MSVC
    localtime_s(&bt, &in_time_t);
#else // Кросс-платформенный, но не потокобезопасный сам по себе вариант.
      // В данном классе он безопасен, так как все вызовы обернуты в блокировку мьютекса.
    bt = *std::localtime(&in_time_t);
#endif

    std::stringstream ss;
    ss << std::put_time(&bt, "%Y-%m-%d %X");
    return ss.str();
}

void Logger::checkFileState() {
    if (!logFile_.is_open()) {
        throw std::runtime_error("Log file is not open");
    }
    
    if (logFile_.fail()) {
        logFile_.clear(); // Сбрасываем флаги ошибок
        throw std::runtime_error("Log file is in failed state");
    }
}