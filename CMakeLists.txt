cmake_minimum_required(VERSION 3.17)
project(hg_new)

set(CMAKE_CXX_STANDARD 20)

add_executable(hg_new main.cpp src/hg.cpp src/hg.h src/libs.h src/partition.cpp src/partition.h src/gc.cpp src/gc.h src/fm.cpp src/fm.h)