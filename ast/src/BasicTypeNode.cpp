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

#include "BasicTypeNode.h"
#include "NodeVisitor.h"

BasicTypeNode::BasicTypeNode(std::string name, const size_t size)
    : TypeNode(), name_(std::move(name)), size_(size)
{
}

BasicTypeNode::~BasicTypeNode() = default;

const std::string& BasicTypeNode::getName() const
{
    return name_;
}

std::string BasicTypeNode::getId() const
{
    return name_;
}

void BasicTypeNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void BasicTypeNode::print(std::ostream& stream) const
{
    stream << "BasicType(" << name_ << ")";
}

std::unique_ptr<BasicTypeNode> BasicTypeNode::makeInt()
{
    return std::make_unique<BasicTypeNode>("INTEGER", 8);
}

std::unique_ptr<BasicTypeNode> BasicTypeNode::makeString()
{
    return std::make_unique<BasicTypeNode>("STRING", 8); // just a pointer
}

std::unique_ptr<BasicTypeNode> BasicTypeNode::makeBool()
{
    return std::make_unique<BasicTypeNode>("BOOLEAN", 8);
}

size_t BasicTypeNode::getByteSize() const
{
    return size_;
}
