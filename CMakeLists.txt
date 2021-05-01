cmake_minimum_required (VERSION 3.11)
project (RobotSimulator)


if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif()

find_package(spdlog REQUIRED)

set(CMAKE_CXX_STANDARD 17)


find_package (Eigen3 3.3 REQUIRED)


set(INCLUDE_DIR
        include
        ${EIGEN3_INCLUDE_DIR})

include_directories(${INCLUDE_DIR})


file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


add_executable(test_main main.cpp ${headers} ${sources})
target_link_libraries(test_main PRIVATE spdlog::spdlog )