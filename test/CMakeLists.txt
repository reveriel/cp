

include_directories()

add_executable(mutex_test mutex_test.cpp)
add_test(NAME mutex_test COMMAND mutex_test)

link_libraries(cp)
add_executable(blocking_queue_bench blocking_queue_bench.cpp)

# address sanitizer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
add_executable(bounded_blocking_queue_bench bounded_blocking_queue_bench.cpp)
