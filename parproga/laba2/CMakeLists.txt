# ==> Ensure proper CMake support

cmake_minimum_required(VERSION 3.10)

# ==> Force new C++ standard (we like new features here)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(pthread_integral VERSION 1.0)

# ==> Add library
add_subdirectory(my_threads)

# ==> Build targets
add_executable(main main.c)
target_link_libraries(main PUBLIC my_threads)
