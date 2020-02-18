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
#include "BlockNode.h"
#include "ParameterDeclarationNode.h"

class ProcedureDeclarationNode : public BlockNode {
  public:
    using ParamDeclList = std::vector<std::unique_ptr<ParameterDeclarationNode>>;

  private:
    ParamDeclList params_;
    ProcDeclList procedures_;

  public:
    ProcedureDeclarationNode(const FilePos& pos,
                             const std::string& name,
                             const std::shared_ptr<Scope>& parent);
    ~ProcedureDeclarationNode() override;

    ParamDeclList& getParams();
    const ParamDeclList& getParams() const;
    ProcDeclList& getProcedures() override;
    const ProcDeclList& getProcedures() const override;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
