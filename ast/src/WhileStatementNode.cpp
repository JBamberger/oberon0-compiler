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

#include "WhileStatementNode.h"
#include "NodeVisitor.h"
#include "PrintUtils.h"
#include <cassert>

WhileStatementNode::WhileStatementNode(const FilePos& pos,
                                       std::unique_ptr<ExpressionNode> condition)
    : StatementNode(pos), condition_(std::move(condition))
{
    assert(condition_ != nullptr);
}

WhileStatementNode::~WhileStatementNode() = default;

const std::unique_ptr<ExpressionNode>& WhileStatementNode::getCondition() const
{
    return condition_;
}

WhileStatementNode::StmtList& WhileStatementNode::getBody()
{
    return body_;
}

const WhileStatementNode::StmtList& WhileStatementNode::getBody() const
{
    return body_;
}

void WhileStatementNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void WhileStatementNode::print(std::ostream& stream) const
{
    stream << "While(" << *condition_ << ", ";
    printList(stream, "Body", body_);
    stream << ")";
}
