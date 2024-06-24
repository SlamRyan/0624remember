#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"

#include <string>

namespace Ins2D5CameraLog {
    class Ins2D5Logger
    {
    public:
        Ins2D5Logger();
        ~Ins2D5Logger();
        static void init();
        static void info(const std::string& msg);
        static void warn(const std::string& msg);
        static void error(const std::string& msg);
        static void debug(const std::string& msg);
        static void critical(const std::string& msg);
    public:
        static std::shared_ptr<spdlog::logger> mpLogger;
        //static std::shared_ptr<spdlog::logger> mpLogger_debug;
    };
}
