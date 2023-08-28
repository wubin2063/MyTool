cmake_minimum_required(VERSION 3.23)
project(IniReader)

set(CMAKE_CXX_STANDARD 17)

add_executable(IniReader main.cpp
        IniReader.cpp
        IniReader.h)
