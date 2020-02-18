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
#include "FieldDeclarationNode.h"
#include "MemberLayout.h"
#include "Scope.h"
#include "TypeNode.h"
#include <vector>

class RecordTypeNode : public TypeNode {
  private:
    std::shared_ptr<Scope> scope_;
    MemberLayout<FieldDeclarationNode> members_;

  public:
    RecordTypeNode(std::shared_ptr<Scope> parent);
    ~RecordTypeNode() override;

    MemberLayout<FieldDeclarationNode>& getMembers();

    const MemberLayout<FieldDeclarationNode>& getMembers() const;

    const std::shared_ptr<Scope>& getScope() const;

    std::string getId() const override;

    size_t getByteSize() const override;

    void visit(NodeVisitor* visitor) const override;

    void print(std::ostream& stream) const override;
};
