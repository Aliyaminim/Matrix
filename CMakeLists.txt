cmake_minimum_required(VERSION 3.13)

project(mymatrix)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source build is forbidden")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -O0 -D DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2 -DNDEBUG")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(GTest REQUIRED)
enable_testing()
add_subdirectory(test)

add_executable(maind ./src/main_double.cpp)
target_include_directories(maind PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(mainll ./src/main_ll.cpp)
target_include_directories(mainll PUBLIC ${CMAKE_SOURCE_DIR}/include)
#cmake -DCMAKE_BUILD_TYPE=Debug