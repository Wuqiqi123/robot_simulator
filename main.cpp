//
// Created by wqq on 2021/4/4.
//

#include "robot_simulator/utils/log.h"

int main(int, char *[]) {
    // init logger
    rs::Logger::get_instance().init("logger/file.txt");
    rs::Logger::get_instance().set_logger_level(rs::LoggerLevel::trace);

    LOG_ERROR("1111, {}", 1);
    LOG_INFO("1111, {}", 1);
    LOG_WARN("1111, {}", 1);
    LOG_DEBUG("1111, {}", 1);

    return 0;
}
