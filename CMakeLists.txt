cmake_minimum_required(VERSION 3.12)
project(oberon0c)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(3rdparty/fmt)
add_subdirectory(util)
add_subdirectory(scanner)
add_subdirectory(parser)


add_executable(oberon0c
    main.cpp
)
target_include_directories(oberon0c PRIVATE
    oberon0c_util
    oberon0c_parser
)
target_link_libraries(oberon0c PUBLIC
    oberon0c_util
    oberon0c_parser
)