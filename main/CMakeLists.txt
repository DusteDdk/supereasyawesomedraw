add_executable(../draw main.c)

target_link_libraries(../draw LINK_PUBLIC
    m
    Util
    SDL2
    SDL2_image
)
