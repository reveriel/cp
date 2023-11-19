cmake_minimum_required(VERSION 3.8.0)
# set(CMAKE_MAKE_PROGRAM /opt/homebrew/bin/ninja)
project(cp VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(src/)

set(CMAKE_CXX_STANDARD 20)


# check thread safety

# use clang
set(CMAKE_CXX_COMPILER "clang++")

# -Wthread-safety
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
 -D_LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS \
 -Wthread-safety -Wall -Wextra -Wpedantic -Werror")
    # "-Wall -Wextra -Wpedantic -Werror -Wthread-safety -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-private-field -Wno-unused-result -Wno-missing-field-initializers -Wno-missing-braces")

add_subdirectory(src)
add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
