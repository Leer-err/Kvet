#ifndef LOGGER_H
#define LOGGER_H

#include <format>
#include <memory>
#include <string>
#include <vector>

namespace spdlog {
namespace sinks {
class sink;
}
class logger;
using sink_ptr = std::shared_ptr<sinks::sink>;
}  // namespace spdlog

enum class LogLevel { Debug, Info, Warn, Error };

class Logger {
   public:
    Logger(const char* name, std::vector<spdlog::sink_ptr> sinks);
    ~Logger();

    void setLevel(LogLevel level);

    template <typename... ARGS>
    void info(std::format_string<ARGS...> line, ARGS&&... args) {
        logInfo(std::format(line, std::forward<ARGS>(args)...));
    }

    template <typename... ARGS>
    void warning(std::format_string<ARGS...> line, ARGS&&... args) {
        logWarning(std::format(line, std::forward<ARGS>(args)...));
    }

    template <typename... ARGS>
    void error(std::format_string<ARGS...> line, ARGS&&... args) {
        logError(std::format(line, std::forward<ARGS>(args)...));
    }

    template <typename... ARGS>
    void debug(std::format_string<ARGS...> line, ARGS&&... args) {
        logDebug(std::format(line, std::forward<ARGS>(args)...));
    }

   private:
    void logInfo(const std::string& line);
    void logWarning(const std::string& line);
    void logError(const std::string& line);
    void logDebug(const std::string& line);

    std::shared_ptr<spdlog::logger> logger;
};

#endif  // LOGGER_H