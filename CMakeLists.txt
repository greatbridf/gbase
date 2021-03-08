cmake_minimum_required(VERSION 3.0.0)
project(gbase VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

include_directories(include)

add_compile_options(-Wall -Wextra -Werror -pedantic)

set(SOURCES src/gbase.cpp
            src/error.cpp
            src/fs/file.cpp
            src/io/string.cpp
            src/net/socket.cpp
            )

set(HEADERS include/gbase/error.hpp
            include/gbase/gbase.hpp
            include/gbase/types.hpp
            include/gbase/fs.hpp
            include/gbase/fs/file.hpp
            include/gbase/io.hpp
            include/gbase/io/string.hpp
            include/gbase/net.hpp
            include/gbase/net/socket_fwd.hpp
            include/gbase/net/basic_socket.hpp
            include/gbase/net/basic_socket_buf.hpp
            include/gbase/net/socket.hpp
            include/gbase/net/socket_types.hpp
            )

add_library(gbase ${SOURCES} ${HEADERS})

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
