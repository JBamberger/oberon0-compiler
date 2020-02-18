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

#include "UnaryExpressionNode.h"
#include "NodeVisitor.h"
#include "Scope.h"
#include "Token.h"
#include <cassert>

UnaryExpressionNode::UnaryExpressionNode(const FilePos& pos,
                                         TypeNode* type,
                                         const UnaryOperator op,
                                         std::unique_ptr<ExpressionNode> operand)
    : ExpressionNode(pos, type), operator_(op), operand_(std::move(operand))
{
    assert(operand_ != nullptr);
}

UnaryExpressionNode::~UnaryExpressionNode() = default;

UnaryOperator UnaryExpressionNode::getOperator() const
{
    return operator_;
}

const std::unique_ptr<ExpressionNode>& UnaryExpressionNode::getOperand() const
{
    return operand_;
}

void UnaryExpressionNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void UnaryExpressionNode::print(std::ostream& stream) const
{
    stream << "UnaryExpression(" << operator_ << ", " << *operand_ << ")";
}
