# ==> Ensure proper CMake support

cmake_minimum_required(VERSION 3.10)

# ==> Force new C++ standard (we like new features here)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mpi_test VERSION 1.0)

# ==> Add library
add_subdirectory(mpi)

# ==> Build targets
add_executable(consequent consequent.cpp)
target_link_libraries(consequent PUBLIC mpi)

add_executable(triangle mpi_triangle.cpp)
target_link_libraries(triangle PUBLIC mpi)

add_executable(centered mpi_centered.cpp)
target_link_libraries(centered PUBLIC mpi)

add_executable(squared mpi_squared.cpp)
target_link_libraries(squared PUBLIC mpi)