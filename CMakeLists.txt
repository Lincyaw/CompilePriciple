cmake_minimum_required(VERSION 3.17)
project(compile)

set(CMAKE_CXX_STANDARD 14)

add_executable(compile main.cpp lexical_analyzer.cpp lexical_analyzer.h Grammar_Analyzer.cpp Grammar_Analyzer.h myhead.h)