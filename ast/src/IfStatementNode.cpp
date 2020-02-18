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

#include "IfStatementNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <cassert>

IfStatementNode::IfStatementNode(const FilePos& pos, std::unique_ptr<ExpressionNode> condition)
    : StatementNode(pos), condition_(std::move(condition))
{
    assert(condition_ != nullptr);
}

IfStatementNode::~IfStatementNode() = default;

const std::unique_ptr<ExpressionNode>& IfStatementNode::getCondition() const
{
    return condition_;
}

IfStatementNode::StmtList& IfStatementNode::getThenPart()
{
    return thenPart_;
}

IfStatementNode::StmtList& IfStatementNode::getElsePart()
{
    return elsePart_;
}

const IfStatementNode::StmtList& IfStatementNode::getThenPart() const
{
    return thenPart_;
}

const IfStatementNode::StmtList& IfStatementNode::getElsePart() const
{
    return elsePart_;
}

void IfStatementNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void IfStatementNode::print(std::ostream& stream) const
{
    stream << "IfStatementNode(" << *condition_ << " then ";
    printList(stream, "ThenBody", thenPart_);
    if (!elsePart_.empty()) {
        stream << " else ";
        printList(stream, "ElseBody", elsePart_);
    }
    stream << ")";
}
