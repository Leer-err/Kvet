#ifndef LOGGER_FACTORY_H
#define LOGGER_FACTORY_H

#include "Logger.h"

class LoggerFactory {
   public:
    static Logger getLogger(const char* name);
    static Logger getLogger(const char* name, LogLevel level);

   private:
    static LoggerFactory get() {
        static LoggerFactory instance;
        return instance;
    }

    LoggerFactory();

    std::vector<spdlog::sink_ptr> sinks;
};

#endif  // LOGGER_FACTORY_H