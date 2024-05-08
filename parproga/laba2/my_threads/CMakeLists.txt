add_library(my_threads STATIC src/method.c src/thread_calculate.c src/thread_work.c)
target_include_directories(my_threads PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)