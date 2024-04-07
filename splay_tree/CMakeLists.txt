cmake_minimum_required(VERSION 3.10)
project(SplayTree VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(SplayTree)

add_executable(main main.cpp)

target_link_libraries(main splay_tree)