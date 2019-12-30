

set(SOURCES
    "include/ExpressionNode.h"
    "include/StatementNode.h"
    "include/Identifier.h"
    "include/TypedIdentifierNode.h"
    "include/TypeNode.h"
    "include/NodeVisitor.h"              "src/NodeVisitor.cpp"
    "include/Node.h"                     "src/Node.cpp"
    "include/UnaryExpressionNode.h"      "src/UnaryExpressionNode.cpp"
    "include/BinaryExpressionNode.h"     "src/BinaryExpressionNode.cpp"
    "include/NumberConstantNode.h"       "src/NumberConstantNode.cpp"
    "include/VariableReferenceNode.h"    "src/VariableReferenceNode.cpp"
    "include/ParameterReferenceNode.h"   "src/ParameterReferenceNode.cpp"
    "include/ArrayReferenceNode.h"       "src/ArrayReferenceNode.cpp"
    "include/WhileStatementNode.h"       "src/WhileStatementNode.cpp"
    "include/IfStatementNode.h"          "src/IfStatementNode.cpp"
    "include/AssignmentNode.h"           "src/AssignmentNode.cpp"
    "include/ProcedureCallNode.h"        "src/ProcedureCallNode.cpp"
    "include/VariableDeclarationNode.h"  "src/VariableDeclarationNode.cpp"
    "include/FieldReferenceNode.h"       "src/FieldReferenceNode.cpp"
    "include/FieldDeclarationNode.h"     "src/FieldDeclarationNode.cpp"
    "include/ParameterDeclarationNode.h" "src/ParameterDeclarationNode.cpp"
    "include/BasicTypeNode.h"            "src/BasicTypeNode.cpp"
    "include/ArrayTypeNode.h"            "src/ArrayTypeNode.cpp"
    "include/RecordTypeNode.h"           "src/RecordTypeNode.cpp"
    "include/ConstantDeclarationNode.h"  "src/ConstantDeclarationNode.cpp"
    "include/BlockNode.h"                "src/BlockNode.cpp"
    "include/ModuleNode.h"               "src/ModuleNode.cpp"
    "include/ProcedureDeclarationNode.h" "src/ProcedureDeclarationNode.cpp"
    "include/TypeDeclarationNode.h"      "src/TypeDeclarationNode.cpp"
    "include/StringConstantNode.h"       "src/StringConstantNode.cpp"
    "include/Scope.h"                    "src/Scope.cpp"
)

add_library(oberon0c_parser_ast ${SOURCES})
target_include_directories(oberon0c_parser_ast PUBLIC "include" oberon0c_scanner)
target_link_libraries(oberon0c_parser_ast PUBLIC oberon0c_scanner)
