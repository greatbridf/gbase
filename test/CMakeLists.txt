cmake_minimum_required(VERSION 3.0.0)

macro(add_test_executable NAME)
    add_executable(${NAME} ${ARGN})
    target_link_libraries(${NAME} gbase)
    add_test(NAME ${NAME} COMMAND ${NAME})
endmacro()

add_test_executable(test_run test_run.cpp)
add_test_executable(test_file test_file.cpp)
add_test_executable(test_exception test_exception.cpp)
add_test_executable(test_serializable test_serializable.cpp)
