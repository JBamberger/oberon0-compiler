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

#include "VariableReferenceNode.h"

#include "NodeVisitor.h"
#include <utility>

VariableReferenceNode::VariableReferenceNode(const FilePos& pos, VariableDeclarationNode* variable)
    : AssignableExpressionNode(pos, variable->getType()), variable_(variable)
{
}

VariableReferenceNode::~VariableReferenceNode() = default;

std::string VariableReferenceNode::getName() const
{
    return variable_->getName();
}

VariableDeclarationNode* VariableReferenceNode::getVariable() const
{
    return variable_;
}

void VariableReferenceNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void VariableReferenceNode::print(std::ostream& stream) const
{
    stream << "VariableReferenceNode(" << getName() << ": " << variable_->getType()->getId() << ")";
}
