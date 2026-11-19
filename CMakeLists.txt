cmake_minimum_required(VERSION 3.23)

project(mechtal)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(mechtal)

add_subdirectory(windowing)
add_subdirectory(input)
