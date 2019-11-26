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
    parser/Parser.h
    parser/Parser.cpp
    parser/ast/Node.h
    parser/ast/Node.cpp
    parser/ast/ExpressionNode.h
    parser/ast/UnaryExpressionNode.h
    parser/ast/UnaryExpressionNode.cpp
    parser/ast/BinaryExpressionNode.h
    parser/ast/BinaryExpressionNode.cpp
    parser/ast/NumberConstantNode.h
    parser/ast/NumberConstantNode.cpp
    parser/ast/VariableReferenceNode.h
    parser/ast/MemberReferenceNode.h
    parser/ast/MemberReferenceNode.cpp
    parser/ast/ArrayReferenceNode.h
    parser/ast/ArrayReferenceNode.cpp
    main.cpp
)
