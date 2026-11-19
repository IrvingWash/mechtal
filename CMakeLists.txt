cmake_minimum_required(VERSION 3.23)

project(mechtal)

add_executable(mechtal)

target_sources(mechtal
    PRIVATE
        mechtal/main.cpp
)
