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
#include "Node.h"

#include "ConstantDeclarationNode.h"
#include "TypeDeclarationNode.h"

#include "MemberLayout.h"
#include "Scope.h"
#include "StatementNode.h"
#include "VariableDeclarationNode.h"
#include <vector>

class ProcedureDeclarationNode;

class BlockNode : public Node {
  public:
    using ConstDeclList = std::vector<std::unique_ptr<ConstantDeclarationNode>>;
    using TypeDeclList = std::vector<std::unique_ptr<TypeDeclarationNode>>;
    using StatementList = std::vector<std::unique_ptr<StatementNode>>;
    using ProcDeclList = std::vector<std::unique_ptr<ProcedureDeclarationNode>>;

  private:
    std::string name_;
    std::shared_ptr<Scope> scope_;
    ConstDeclList constants_;
    MemberLayout<VariableDeclarationNode> variables_;
    TypeDeclList types_;
    StatementList statements_;

  public:
    BlockNode(const FilePos& pos, std::string name, std::shared_ptr<Scope> scope);
    ~BlockNode() override;

    const std::string& getName() const;
    StatementList& getStatements();
    ConstDeclList& getConstants();
    MemberLayout<VariableDeclarationNode>& getVariables();
    TypeDeclList& getTypes();
    virtual ProcDeclList& getProcedures() = 0;

    const StatementList& getStatements() const;
    const ConstDeclList& getConstants() const;
    const MemberLayout<VariableDeclarationNode>& getVariables() const;
    const TypeDeclList& getTypes() const;
    virtual const ProcDeclList& getProcedures() const = 0;

    const std::shared_ptr<Scope>& getScope() const;
    void print(std::ostream& stream) const override;
};
