add_library(mpi STATIC src/equation.cpp src/io.cpp src/calculate.cpp)
target_include_directories(mpi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)