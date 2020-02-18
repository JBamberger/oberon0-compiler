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

#include "CodeGenerator.h"
#include "NodeVisitor.h"
#include <ThrowingNodeVisitor.h>

/**
 * Simple stack-based x86_64 code generator.
 */
class X86_64CodeGenerator : public CodeGenerator, private ThrowingNodeVisitor {
    std::ostream* output_;
    std::string nl_;
    size_t label_nr_;
    std::shared_ptr<Scope> root_scope;
    bool should_deref = true;

  public:
    X86_64CodeGenerator();

    ~X86_64CodeGenerator() override;

    void generate(std::unique_ptr<ModuleNode> ast, std::ostream* output) override;

  private:
    std::string nextLabel();

    void defineConstants(const BlockNode* node);
    void defineVariables(const BlockNode* node);

    void visit(const ArrayReferenceNode* node) override;

    void visit(const AssignmentNode* node) override;

    void visit(const BinaryExpressionNode* node) override;

    void visit(const UnaryExpressionNode* node) override;

    void visit(const ConstantDeclarationNode* node) override;

    void visit(const VariableDeclarationNode* node) override;

    void visit(const ProcedureDeclarationNode* node) override;

    void visit(const FieldReferenceNode* node) override;

    void visit(const FieldDeclarationNode* node) override;

    void visit(const IfStatementNode* node) override;

    void visit(const ModuleNode* node) override;

    void visit(const NumberConstantNode* node) override;

    void visit(const StringConstantNode* node) override;

    void visit(const ParameterDeclarationNode* node) override;

    void visit(const ProcedureCallNode* node) override;

    void visit(const TypedIdentifierNode* node) override;

    void visit(const VariableReferenceNode* node) override;

    void visit(const WhileStatementNode* node) override;
};
