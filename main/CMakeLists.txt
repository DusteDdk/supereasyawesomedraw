add_executable(main main.c)

target_link_libraries(main LINK_PUBLIC
    m
    Util
    SDL2
    SDL2_image
)
