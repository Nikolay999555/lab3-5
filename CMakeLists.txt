set(CMAKE_VER 3.12)
cmake_minimum_required(VERSION ${CMAKE_VER})
project(labs)

set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_SOURCE_DIR})

find_package(OpenMP REQUIRED)
add_subdirectory(src)

include(cmake/GetGoogletest.cmake)
find_package(Threads REQUIRED)
enable_testing()
add_subdirectory(test)
