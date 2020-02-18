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

#include "ArrayReferenceNode.h"
#include "NodeVisitor.h"
#include <cassert>
#include <utility>

ArrayReferenceNode::ArrayReferenceNode(const FilePos& pos,
                                       std::unique_ptr<ExpressionNode> index,
                                       TypeNode* type,
                                       std::unique_ptr<AssignableExpressionNode> array_ref)
    : AssignableExpressionNode(pos, type), array_ref_(std::move(array_ref)),
      index_(std::move(index))
{
    assert(array_ref_ != nullptr);
    assert(index_ != nullptr);
}

ArrayReferenceNode::~ArrayReferenceNode() = default;

const std::unique_ptr<AssignableExpressionNode>& ArrayReferenceNode::getArrayRef() const
{
    return array_ref_;
}

const std::unique_ptr<ExpressionNode>& ArrayReferenceNode::getIndex() const
{
    return index_;
}

void ArrayReferenceNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void ArrayReferenceNode::print(std::ostream& stream) const
{
    stream << "ArrayReferenceNode(" << *array_ref_ << ", " << *index_ << ")";
}
