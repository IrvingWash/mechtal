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

if (NOT TARGET SDL3::SDL3) 
    set(SDL_SHARED       OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC       ON  CACHE BOOL "" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG release-3.4.14
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL3)
endif()

if (NOT TARGET Vulkan::Headers)
    FetchContent_Declare(VulkanHeaders
        GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers
        GIT_TAG        vulkan-sdk-1.4.357.0
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(VulkanHeaders)
endif()

target_link_libraries(windowing
    PRIVATE Vulkan::Headers
    PRIVATE SDL3::SDL3
)
