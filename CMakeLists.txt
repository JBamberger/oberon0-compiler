cmake_minimum_required(VERSION 3.12)
project(oberon0c)

set(CMAKE_CXX_STANDARD 14)

include_directories(scanner)
include_directories(util)

add_executable(oberon0c
        scanner/IdentToken.cpp
        scanner/IdentToken.h
        scanner/NumberToken.cpp
        scanner/NumberToken.h
        scanner/Scanner.cpp
        scanner/Scanner.h
        scanner/StringToken.cpp
        scanner/StringToken.h
        scanner/Token.cpp
        scanner/Token.h
        util/Logger.cpp
        util/Logger.h
        main.cpp parser/Parser.h parser/ast/Node.h parser/Parser.cpp)
