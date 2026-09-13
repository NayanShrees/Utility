#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <format>
#include <fstream>
#include <iostream>
#include <string_view>

#ifdef DEVELOPMENT_MODE
#define LOG_DEBUG(...) Logger::log(LogLevel::DEBUG, __func__, __VA_ARGS__)
#define LOG_INFO(...) Logger::log(LogLevel::INFO, __func__, __VA_ARGS__)
#else
#define LOG_DEBUG(...) \
    do {               \
    } while (0)
#define LOG_INFO(...) \
    do {              \
    } while (0)
#endif
#define LOG_WARN(...) Logger::log(LogLevel::WARN, __func__, __VA_ARGS__)
#define LOG_ERROR(...) Logger::log(LogLevel::ERROR, __func__, __VA_ARGS__)

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
};

class Logger {
   private:
    inline static std::ofstream fileStream;
    inline static bool logToFile = false;

    static constexpr std::string_view levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG:
                return "[DEBUG]";
            case LogLevel::INFO:
                return "[INFO]";
            case LogLevel::WARN:
                return "[WARN]";
            case LogLevel::ERROR:
                return "[ERROR]";
        }
        return "[UNKNOWN]";
    }

   public:
    static void initializeFileLogging(const std::string& filepath) {
        fileStream.open(filepath, std::ios::out | std::ios::app);
        if (fileStream.is_open()) {
            logToFile = true;
            LOG_INFO("Logging to file: {}", filepath);
        } else {
            std::cerr << "[LOGGER ERROR] Failed to open log file: " << filepath << '\n';
        }
    }

    template <typename... Args>
    static void log(LogLevel level, std::string_view func, std::format_string<Args...> format,
                    Args&&... args) {
        const auto message = std::format(format, std::forward<Args>(args)...);
        const auto log_message = std::format("{} [{}] {}\n", levelToString(level), func, message);
        if (level == LogLevel::ERROR) {
            std::cerr << log_message;
        } else {
            std::cout << log_message;
        }

        if (logToFile && fileStream.is_open()) {
            fileStream << log_message;
            fileStream.flush();
        }
    }
};

#endif  // LOGGER_HPP_
