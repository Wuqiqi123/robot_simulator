cmake_minimum_required (VERSION 3.11)
project (RobotSimulator)

find_package(spdlog REQUIRED)

set(CMAKE_CXX_STANDARD 17)


include_directories(include)

add_executable(test_main main.cpp include/robot_simulator/utils/log.h)
target_link_libraries(test_main PRIVATE spdlog::spdlog )