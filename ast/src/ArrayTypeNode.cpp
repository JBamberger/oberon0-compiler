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

#include "ArrayTypeNode.h"
#include "NodeVisitor.h"

ArrayTypeNode::ArrayTypeNode(const int size, TypeNode* type) : TypeNode(), size_(size), type_(type)
{
}

ArrayTypeNode::~ArrayTypeNode() = default;

int ArrayTypeNode::getSize() const
{
    return size_;
}

TypeNode* ArrayTypeNode::getType() const
{
    return type_;
}

std::string ArrayTypeNode::getId() const
{
    return "[A," + std::to_string(size_) + "," + type_->getId() + "]";
}

void ArrayTypeNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void ArrayTypeNode::print(std::ostream& stream) const
{
    stream << "ArrayTypeNode(" << size_ << "," << type_ << ")";
}

size_t ArrayTypeNode::getByteSize() const
{
    return size_ * type_->getByteSize();
}
