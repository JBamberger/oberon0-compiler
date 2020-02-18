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

#include "TypeDeclarationNode.h"
#include "NodeVisitor.h"
#include <utility>

TypeDeclarationNode::TypeDeclarationNode(const FilePos& pos, std::string name, TypeNode* type)
    : Node(pos), name_(std::move(name)), type_(type)
{
}

TypeDeclarationNode::~TypeDeclarationNode() = default;

std::string TypeDeclarationNode::getName() const
{
    return name_;
}

TypeNode* TypeDeclarationNode::getType() const
{
    return type_;
}

void TypeDeclarationNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void TypeDeclarationNode::print(std::ostream& stream) const
{
    stream << "TypeDeclarationNode(" << name_ << ", " << type_ << ")";
}
