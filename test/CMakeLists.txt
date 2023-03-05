

include_directories(../)

add_executable(mutex_test mutex_test.cpp)
add_test(NAME mutex_test COMMAND mutex_test)
