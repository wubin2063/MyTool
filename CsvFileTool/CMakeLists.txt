cmake_minimum_required(VERSION 3.23)
project(test)

set(CMAKE_CXX_STANDARD 17)

add_executable(test main.cpp CsvParser.h CsvPaser.cpp)
