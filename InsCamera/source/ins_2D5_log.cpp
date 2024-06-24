#include "../include/ins_2D5_log.h"


namespace Ins2D5CameraLog {
    std::shared_ptr<spdlog::logger> Ins2D5Logger::mpLogger = nullptr;
    //std::shared_ptr<spdlog::logger> Ins2D5Logger::mpLogger_debug = nullptr;

    Ins2D5Logger::Ins2D5Logger()
    {
        init();
    }

    Ins2D5Logger::~Ins2D5Logger()
    {
    }

    void Ins2D5Logger::init()
    {
        if (!mpLogger) {
            mpLogger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 0, 0);
            spdlog::flush_every(std::chrono::seconds(1));

            //mpLogger_debug = spdlog::daily_logger_mt("daily_debug_logger", "logs/debug.txt", 0, 0);
            //spdlog::flush_every(std::chrono::seconds(1));
        }
    }

    void Ins2D5Logger::info(const std::string& msg)
    {
        if (mpLogger) {
            mpLogger->info(msg);
        }
    }

    void Ins2D5Logger::error(const std::string& msg)
    {
        if (mpLogger) {
            mpLogger->error(msg);
        }
    }

    void Ins2D5Logger::warn(const std::string& msg)
    {
        if (mpLogger) {
            mpLogger->warn(msg);
        }
    }

    void Ins2D5Logger::debug(const std::string& msg)
    {
        if (mpLogger) {
            mpLogger->debug(msg);
        }
    }

    void Ins2D5Logger::critical(const std::string& msg)
    {
        if (mpLogger) {
            mpLogger->critical(msg);
        }
     }

}
