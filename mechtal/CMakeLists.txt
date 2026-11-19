add_executable(mechtal)

target_sources(mechtal
    PRIVATE
        main.cpp
)

target_link_libraries(mechtal
    PRIVATE
        windowing
)
