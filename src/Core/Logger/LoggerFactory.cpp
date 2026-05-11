#include "LoggerFactory.h"

#include <chrono>
#include <cstddef>
#include <vector>

#include "Logger.h"
#include "spdlog/cfg/env.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

constexpr char LEVELS_ENV_VAR[] = "LOG_LEVEL";

LoggerFactory::LoggerFactory() {
    constexpr size_t MAX_SIZE = 1 << 22;
    constexpr uint32_t MAX_FILE_COUNT = 5;

    spdlog::flush_every(std::chrono::seconds(1));

    sinks = {std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
             std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                 "logs/log.txt", MAX_SIZE, MAX_FILE_COUNT, true)};
}

Logger LoggerFactory::getLogger(const char* name) {
    auto logger = Logger(name, get().sinks);

    spdlog::cfg::load_env_levels(LEVELS_ENV_VAR);

    return logger;
}

Logger LoggerFactory::getLogger(const char* name, LogLevel level) {
    auto logger = Logger(name, get().sinks);
    logger.setLevel(level);

    spdlog::cfg::load_env_levels(LEVELS_ENV_VAR);

    return logger;
}