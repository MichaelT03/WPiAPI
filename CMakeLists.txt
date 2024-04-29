cmake_minimum_required(VERSION 3.16.3)

project(prog)

add_executable(${PROJECT_NAME} main.cpp piTools.cpp)

find_library(REQUIRED wiringPi)

target_link_libraries(${PROJECT_NAME} PRIVATE wiringPi pthread)

target_include_directories(${PROJECT_NAME} PRIVATE ../WPiAPI/)
