add_library(windowing)

target_sources(windowing
    PRIVATE
        windowing.cpp

    PUBLIC
        FILE_SET HEADERS
        FILES
            windowing.hpp
)

include(FetchContent)
FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.4.14
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL3)

target_link_libraries(windowing PRIVATE SDL3::SDL3)
