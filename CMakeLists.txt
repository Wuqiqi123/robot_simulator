cmake_minimum_required (VERSION 3.11)
project (RobotSimulator)

find_package(spdlog REQUIRED)

set(CMAKE_CXX_STANDARD 17)


find_package (Eigen3 3.3 REQUIRED)



include_directories(include ${EIGEN3_INCLUDE_DIR})

add_executable(test_main main.cpp include/robot_simulator/utils/log.h)
target_link_libraries(test_main PRIVATE spdlog::spdlog )