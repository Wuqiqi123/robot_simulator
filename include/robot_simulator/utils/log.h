// Copyright(c) 2020-2021, Qiqi Wu<1258552199@qq.com>.
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// This file is create by Qiqi Wu, 2021/4/7

#pragma once

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#endif

#include <assert.h>

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <filesystem>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/logger.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/bundled/printf.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>


namespace rs {

enum class LoggerLevel {
    trace,
    debug,
    info,
    warn,
    err,
    critical,
    off
};

class Logger {
public:

    static Logger &get_instance() {
        static Logger m_instance;
        return m_instance;
    }

    auto get_logger() { return m_logger; }

    void set_logger_level(LoggerLevel level){
        switch (level)
        {
            case LoggerLevel::trace:
                m_logger->set_level(spdlog::level::trace);
                break;
            case LoggerLevel::debug:
                m_logger->set_level(spdlog::level::debug);
                break;
            case LoggerLevel::info:
                m_logger->set_level(spdlog::level::info);
                break;
            case LoggerLevel::warn:
                m_logger->set_level(spdlog::level::warn);
                break;
            case LoggerLevel::err:
                m_logger->set_level(spdlog::level::err);
                break;
            case LoggerLevel::critical:
                m_logger->set_level(spdlog::level::critical);
                break;
            case LoggerLevel::off:
                m_logger->set_level(spdlog::level::off);
                break;
        }
    }

    static void shutdown() { spdlog::shutdown(); }

public:
    bool init(std::string_view log_file_path) {
        if (is_init_) {
            return true;
        }
        try {
            // check log path and try to create log directory
            std::filesystem::path log_path(log_file_path);
            std::filesystem::path log_dir = log_path.parent_path();
            if (!std::filesystem::exists(log_path)) {
                std::filesystem::create_directories(log_dir);
            }

            // initialize spdlog
            constexpr std::size_t log_buffer_size = 32 * 1024; // 32kb
            spdlog::init_thread_pool(8192, std::thread::hardware_concurrency());

            std::vector<spdlog::sink_ptr> sink_list;
            //add stdout sink
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::debug);
            //consoleSink->set_pattern("[multi_sink_example] [%^%l%$] %v");
//            console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%l][%s:%#] %v");
            sink_list.push_back(console_sink);

            // add file sink
            auto basic_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/basic_sink.log");
            basic_sink->set_level(spdlog::level::debug);
            sink_list.push_back(basic_sink);

            // add rotate file sink
            auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/rotatingSink.log", 1024 * 1024,
                                                                                   5, false);
            rotating->set_level(spdlog::level::debug);
//            rotating->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%5l%$][thread:%t]  %v");
            sink_list.push_back(rotating);

            // set async log
            m_logger = std::make_shared<spdlog::async_logger>("d", begin(sink_list), end(sink_list),
                                                              spdlog::thread_pool());
            //register it if you need to access it globally
            spdlog::register_logger(m_logger);

            // set log level
#ifdef _DEBUG
            m_logger->set_level(spdlog::level::trace);
#else
            m_logger->set_level(spdlog::level::debug);
#endif

            m_logger->flush_on(spdlog::level::warn);
            spdlog::flush_every(std::chrono::seconds(3));
        }
        catch (...)
        {
            std::cout<< "Init logger failed";
            return false;
        }

        is_init_ = true;
        return true;

    }

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

private:
    Logger() = default;
    ~Logger() = default;

    std::atomic_bool is_init_{false};
    std::shared_ptr<spdlog::async_logger> m_logger;
};

} // end of namespace rs


// use fmt lib, e.g. LOG_WARN("warn log, {1}, {1}, {2}", 1, 2);
#define LOG_TRACE(msg,...) \
{ if (rs::Logger::get_instance().get_logger()->level() == spdlog::level::trace) \
    rs::Logger::get_instance().get_logger()->log({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::trace, msg, ##__VA_ARGS__);  \
};

#define LOG_DEBUG(msg,...) rs::Logger::get_instance().get_logger()->log({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::debug, msg, ##__VA_ARGS__)
#define LOG_INFO(msg,...)  rs::Logger::get_instance().get_logger()->log({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::info, msg, ##__VA_ARGS__)
#define LOG_WARN(msg,...)  rs::Logger::get_instance().get_logger()->log({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::warn, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg,...) rs::Logger::get_instance().get_logger()->log({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::err, msg, ##__VA_ARGS__)
#define LOG_FATAL(msg,...) rs::Logger::get_instance().get_logger()->log({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::critical, msg, ##__VA_ARGS__)