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

#include "VariableDeclarationNode.h"

#include "NodeVisitor.h"
#include <utility>

VariableDeclarationNode::VariableDeclarationNode(const FilePos& pos,
                                                 std::string name,
                                                 TypeNode* type,
                                                 BlockNode* parent)
    : TypedIdentifierNode(pos, std::move(name), type), parent_(parent)
{
}

VariableDeclarationNode::~VariableDeclarationNode() = default;

void VariableDeclarationNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void VariableDeclarationNode::print(std::ostream& stream) const
{
    stream << "VariableNode(" << name_ << ", " << type_ << ")";
}
BlockNode* VariableDeclarationNode::getParent() const
{
    return parent_;
}
