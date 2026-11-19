cmake_minimum_required(VERSION 3.23)

project(mechtal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Debug by default
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

option(SANITIZE "ASan + UBSan in Debug builds" ON)

# "Library" to share warnings, etc
add_library(project_settings INTERFACE)

set(GCC_CLANG_WARNINGS
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
    -Wold-style-cast
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
    -Wimplicit-fallthrough
    -Wcast-align
)
set(MSVC_WARNINGS /W4 /permissive-)

target_compile_options(project_settings INTERFACE
    "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:${GCC_CLANG_WARNINGS}>"
    "$<$<CXX_COMPILER_ID:MSVC>:${MSVC_WARNINGS}>"
)
target_compile_definitions(project_settings INTERFACE
    "$<$<CONFIG:Debug>:_GLIBCXX_ASSERTIONS>"
    "$<$<CONFIG:Debug>:_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE>"
)

if(SANITIZE)
    if(MSVC)
        target_compile_options(project_settings INTERFACE "$<$<CONFIG:Debug>:/fsanitize=address>")
    else()
        set(SANITIZER_FLAGS
            -fsanitize=address,undefined
            -fno-sanitize-recover=undefined
            -fno-omit-frame-pointer
        )
        target_compile_options(project_settings INTERFACE "$<$<CONFIG:Debug>:${SANITIZER_FLAGS}>")
        target_link_options(project_settings INTERFACE "$<$<CONFIG:Debug>:-fsanitize=address,undefined>")
    endif()
endif()

add_subdirectory(mechtal)

add_subdirectory(windowing)
add_subdirectory(input)
add_subdirectory(gltf)

foreach(target IN ITEMS mechtal windowing input gltf)
    target_link_libraries(${target} PRIVATE project_settings)
endforeach()
