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

#include "ConstantDeclarationNode.h"
#include "NodeVisitor.h"
#include <cassert>

ConstantDeclarationNode::ConstantDeclarationNode(const FilePos& pos,
                                                 std::string name,
                                                 std::unique_ptr<ConstantNode> value)
    : Node(pos), name_(std::move(name)), value_(std::move(value))
{
    assert(value_ != nullptr);
}

ConstantDeclarationNode::~ConstantDeclarationNode() = default;

const std::string& ConstantDeclarationNode::getName() const
{
    return name_;
}

const std::unique_ptr<ConstantNode>& ConstantDeclarationNode::getValue() const
{
    return value_;
}

void ConstantDeclarationNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void ConstantDeclarationNode::print(std::ostream& stream) const
{
    stream << "ConstantDeclarationNode(" << name_ << ", " << *value_ << ")";
}
