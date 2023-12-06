project(Testing)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(test1 test1.cpp)
target_link_libraries(test1 gtest gtest_main Threads::Threads)

add_executable(test2 test.cpp)
target_link_directories(test2 PUBLIC ${CMAKE_SOURCE_DIR}/include)