cmake_minimum_required(VERSION 3.0.0)
project(gbase VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(include)

set(SOURCES src/gbase.cpp
            )

add_library(gbase ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(test)

# install

install(
    TARGETS gbase
    LIBRARY DESTINATION lib
)

install(
    DIRECTORY include/gbase
    DESTINATION include
)
