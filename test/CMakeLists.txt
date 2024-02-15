

include_directories()

add_executable(mutex_test mutex_test.cpp)
add_test(NAME mutex_test COMMAND mutex_test)

link_libraries(cp)
add_executable(blocking_queue_bench blocking_queue_bench.cpp)

# address sanitizer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
add_executable(bounded_blocking_queue_bench bounded_blocking_queue_bench.cpp)

add_executable(async_log_test async_log_test.cpp)
add_test(NAME async_log_test COMMAND async_log_test)

add_executable(log_file_test log_file_test.cpp)
add_test(NAME log_file_test COMMAND log_file_test)

add_executable(log_stream_test log_stream_test.cpp)
add_test(NAME log_stream_test COMMAND log_stream_test)
link_libraries(gtest_main)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)
FetchContent_MakeAvailable(googletest)
target_link_libraries(log_stream_test gtest_main)