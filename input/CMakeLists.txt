add_library(input)

target_sources(input
    PRIVATE
        input.cpp

    PUBLIC
        FILE_SET HEADERS
        FILES
            input.hpp
)

set(SDL_SHARED       OFF CACHE BOOL "" FORCE)
set(SDL_STATIC       ON  CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
include(FetchContent)
FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.4.14
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL3)

target_link_libraries(input PRIVATE SDL3::SDL3)
