cmake_minimum_required(VERSION 3.0.0)

add_executable(test_run test_run.cpp)

target_link_libraries(test_run gbase)

add_test(NAME test_run COMMAND test_run)
