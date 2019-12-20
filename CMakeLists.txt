cmake_minimum_required(VERSION 3.12)
project(oberon0c)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(3rdparty/fmt)


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
    parser/PrintVisitor.h
    parser/PrintVisitor.cpp
    parser/ElaborationErrors.h

    parser/ast/NodeVisitor.h
    parser/ast/NodeVisitor.cpp
    parser/ast/Node.h
    parser/ast/Node.cpp
    parser/ast/ExpressionNode.h
    parser/ast/UnaryExpressionNode.h
    parser/ast/UnaryExpressionNode.cpp
    parser/ast/BinaryExpressionNode.h
    parser/ast/BinaryExpressionNode.cpp
    parser/ast/NumberConstantNode.h
    parser/ast/NumberConstantNode.cpp
    parser/ast/SelectorNode.h
    parser/ast/VariableReferenceNode.h
    parser/ast/VariableReferenceNode.cpp
    parser/ast/ArrayReferenceNode.h
    parser/ast/ArrayReferenceNode.cpp
    parser/ast/StatementNode.h
    parser/ast/WhileStatementNode.h
    parser/ast/WhileStatementNode.cpp
    parser/ast/IfStatementNode.h
    parser/ast/IfStatementNode.cpp
    parser/ast/AssignmentNode.h
    parser/ast/AssignmentNode.cpp
    parser/ast/ProcedureCallNode.h
    parser/ast/ProcedureCallNode.cpp
    parser/ast/ActualParameterNode.h
    parser/ast/ActualParameterNode.cpp
    parser/ast/IdentifierListNode.h
    parser/ast/IdentifierListNode.cpp
    parser/ast/TypedIdentifierNode.h
    parser/ast/VariableNode.h
    parser/ast/VariableNode.cpp
    parser/ast/FieldReferenceNode.h
    parser/ast/FieldReferenceNode.cpp
    parser/ast/FieldNode.h
    parser/ast/FieldNode.cpp
    parser/ast/ParameterNode.h
    parser/ast/ParameterNode.cpp
    parser/ast/TypedIdentifierListNode.h
    parser/ast/TypeNode.h
    parser/ast/TypeReferenceNode.h
    parser/ast/TypeReferenceNode.cpp
    parser/ast/BasicTypeNode.h
    parser/ast/BasicTypeNode.cpp
    parser/ast/ArrayTypeNode.h
    parser/ast/ArrayTypeNode.cpp
    parser/ast/RecordTypeNode.h
    parser/ast/RecordTypeNode.cpp
    parser/ast/ConstantDeclarationNode.h
    parser/ast/ConstantDeclarationNode.cpp
    parser/ast/BlockNode.h
    parser/ast/BlockNode.cpp
    parser/ast/ModuleNode.h
    parser/ast/ModuleNode.cpp
    parser/ast/ProcedureDeclarationNode.h
    parser/ast/ProcedureDeclarationNode.cpp
    parser/ast/TypeDeclarationNode.h
    parser/ast/TypeDeclarationNode.cpp
    parser/ast/StringConstantNode.h
    parser/ast/StringConstantNode.cpp
    parser/ast/Scope.h
    parser/ast/Scope.cpp

    main.cpp
)

target_link_libraries(oberon0c fmt::fmt)