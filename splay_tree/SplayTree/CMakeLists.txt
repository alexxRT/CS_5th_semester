add_library(splay_tree STATIC src/splay_tree_debug.cpp)

target_include_directories(splay_tree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)