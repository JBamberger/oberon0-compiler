#
# Copyright 2020 Jannik Bamberger
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#



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
    "include/MemberLayout.h"
    "include/ThrowingNodeVisitor.h"
)

add_library(oberon0c_ast ${SOURCES})
target_include_directories(oberon0c_ast PUBLIC "include" oberon0c_scanner)
target_link_libraries(oberon0c_ast PUBLIC oberon0c_scanner)
