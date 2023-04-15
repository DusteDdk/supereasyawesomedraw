add_library (Util util.c)

target_include_directories(Util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Util m)