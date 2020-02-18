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

#include "AssignmentNode.h"
#include "NodeVisitor.h"
#include <cassert>

AssignmentNode::AssignmentNode(const FilePos& pos,
                               std::unique_ptr<AssignableExpressionNode> assignee,
                               std::unique_ptr<ExpressionNode> value)
    : StatementNode(pos), assignee_(std::move(assignee)), value_(std::move(value))
{
    assert(assignee_ != nullptr);
    assert(value_ != nullptr);
}

AssignmentNode::~AssignmentNode() = default;

const std::unique_ptr<AssignableExpressionNode>& AssignmentNode::getAssignee() const
{
    return assignee_;
}

const std::unique_ptr<ExpressionNode>& AssignmentNode::getValue() const
{
    return value_;
}

void AssignmentNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void AssignmentNode::print(std::ostream& stream) const
{
    stream << "AssignmentNode(" << *assignee_ << " := " << *value_ << ")";
}
