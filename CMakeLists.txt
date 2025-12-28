cmake_minimum_required(VERSION 4.0)
project(fum_first_project)

set(CMAKE_CXX_STANDARD 14)

add_executable(program1 1.cpp)
add_executable(program2 2.cpp DSU.h interpreter.h)
add_executable(program3 3.cpp mazegenerator.h interpreter.h)
add_executable(program4 4.cpp mazegenerator.h interpreter.h includer.h DSU.h)