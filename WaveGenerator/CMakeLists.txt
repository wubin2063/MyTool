cmake_minimum_required(VERSION 3.23)
project(WaveGenerator)

set(CMAKE_CXX_STANDARD 17)
include_directories(../FileTool)
add_executable(WaveGenerator main.cpp WaveGenerator.cpp WaveGenerator.h)
