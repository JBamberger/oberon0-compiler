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
#include "NodeVisitor.h"
#include <ostream>

class PrintVisitor : public NodeVisitor {
    std::ostream& out_;
    int indent_;

    std::ostream& line() const;
    void inc();
    void dec();

  public:
    explicit PrintVisitor(std::ostream& out);
    ~PrintVisitor() override;
    void visit(const ArrayReferenceNode* node) override;
    void visit(const ArrayTypeNode* node) override;
    void visit(const AssignmentNode* node) override;
    void visit(const BasicTypeNode* node) override;
    void visit(const BinaryExpressionNode* node) override;
    void visit(const FieldReferenceNode* node) override;
    void visit(const ConstantDeclarationNode* node) override;
    void visit(const FieldDeclarationNode* node) override;
    void visit(const IfStatementNode* node) override;
    void visit(const ModuleNode* node) override;
    void visit(const NumberConstantNode* node) override;
    void visit(const ParameterDeclarationNode* node) override;
    void visit(const ProcedureCallNode* node) override;
    void visit(const ProcedureDeclarationNode* node) override;
    void visit(const RecordTypeNode* node) override;
    void visit(const StringConstantNode* node) override;
    void visit(const TypeDeclarationNode* node) override;
    void visit(const TypedIdentifierNode* node) override;
    void visit(const UnaryExpressionNode* node) override;
    void visit(const VariableDeclarationNode* node) override;
    void visit(const VariableReferenceNode* node) override;
    void visit(const ParameterReferenceNode* node) override;
    void visit(const WhileStatementNode* node) override;
    void visit(const BooleanConstantNode* node) override;

  private:
    void printBlock(const BlockNode* node);
};
