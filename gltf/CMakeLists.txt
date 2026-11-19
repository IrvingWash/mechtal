add_library(gltf)

target_sources(gltf
    PRIVATE
        gltf.cpp

    PUBLIC
        FILE_SET HEADERS
        FILES
            gltf.hpp
)

include(FetchContent)

if(NOT TARGET glaze::glaze)
    FetchContent_Declare(glaze
        GIT_REPOSITORY https://github.com/stephenberry/glaze
        GIT_TAG        v7.2.1
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(glaze)
endif()

target_compile_features(gltf PRIVATE cxx_std_23)
target_link_libraries(gltf PRIVATE glaze::glaze)
