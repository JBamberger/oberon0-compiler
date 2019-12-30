cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(oberon0c VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(3rdparty/fmt)
add_subdirectory(util)
add_subdirectory(scanner)
add_subdirectory(ast)
add_subdirectory(parser)

set(SOURCES
    "main.cpp"
)

add_executable(            oberon0c ${SOURCES})
target_include_directories(oberon0c PRIVATE oberon0c_parser)
target_link_libraries(     oberon0c PRIVATE oberon0c_parser)
