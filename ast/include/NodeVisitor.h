/**
 * Copyright 2020 Jannik Bamberger
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

class BooleanConstantNode;
class ParameterReferenceNode;
class BlockNode;
class FieldReferenceNode;
class SelectorNode;
class FieldDeclarationNode;
class ParameterDeclarationNode;
class ConstantDeclarationNode;
class IfStatementNode;
class NumberConstantNode;
class ProcedureBodyNode;
class ProcedureCallNode;
class ProcedureDeclarationNode;
class ProcedureHeadingNode;
class RecordTypeNode;
class StringConstantNode;
class TypeDeclarationNode;
class TypedIdentifierNode;
class UnaryExpressionNode;
class VariableDeclarationNode;
class VariableReferenceNode;
class WhileStatementNode;
class BinaryExpressionNode;
class ChainedReferenceNode;
class AssignmentNode;
class BasicTypeNode;
class ArrayTypeNode;
class ArrayReferenceNode;
class ModuleNode;

class NodeVisitor {

  public:
    virtual ~NodeVisitor() = 0;

    virtual void visit(const ArrayReferenceNode* node) = 0;
    virtual void visit(const ArrayTypeNode* node) = 0;
    virtual void visit(const AssignmentNode* node) = 0;

    virtual void visit(const BinaryExpressionNode* node) = 0;
    virtual void visit(const UnaryExpressionNode* node) = 0;

    virtual void visit(const ConstantDeclarationNode* node) = 0;
    virtual void visit(const TypeDeclarationNode* node) = 0;
    virtual void visit(const VariableDeclarationNode* node) = 0;
    virtual void visit(const ProcedureDeclarationNode* node) = 0;

    virtual void visit(const FieldReferenceNode* node) = 0;
    virtual void visit(const FieldDeclarationNode* node) = 0;
    virtual void visit(const IfStatementNode* node) = 0;
    virtual void visit(const ModuleNode* node) = 0;
    virtual void visit(const NumberConstantNode* node) = 0;
    virtual void visit(const BooleanConstantNode* node);
    virtual void visit(const StringConstantNode* node) = 0;

    virtual void visit(const ParameterDeclarationNode* node) = 0;
    virtual void visit(const ProcedureCallNode* node) = 0;
    virtual void visit(const RecordTypeNode* node) = 0;
    virtual void visit(const TypedIdentifierNode* node) = 0;
    virtual void visit(const BasicTypeNode* node) = 0;
    virtual void visit(const VariableReferenceNode* node) = 0;
    virtual void visit(const ParameterReferenceNode* node);
    virtual void visit(const WhileStatementNode* node) = 0;
};
