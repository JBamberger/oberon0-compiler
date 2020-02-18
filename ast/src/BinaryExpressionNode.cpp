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

#include "BinaryExpressionNode.h"
#include "NodeVisitor.h"
#include "Scope.h"
#include <cassert>

BinaryExpressionNode::BinaryExpressionNode(const FilePos& pos,
                                           TypeNode* type,
                                           const BinaryOperator op,
                                           std::unique_ptr<ExpressionNode> operand1,
                                           std::unique_ptr<ExpressionNode> operand2)
    : ExpressionNode(pos, type), op_(op), operand1_(std::move(operand1)),
      operand2_(std::move(operand2))
{
    assert(operand1_ != nullptr);
    assert(operand2_ != nullptr);
    assert(operand1_->getType() == operand2_->getType());
}

BinaryExpressionNode::~BinaryExpressionNode() = default;

BinaryOperator BinaryExpressionNode::getOperator() const
{
    return op_;
}

const std::unique_ptr<ExpressionNode>& BinaryExpressionNode::getOperand1() const
{
    return operand1_;
}

const std::unique_ptr<ExpressionNode>& BinaryExpressionNode::getOperand2() const
{
    return operand2_;
}

void BinaryExpressionNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void BinaryExpressionNode::print(std::ostream& stream) const
{
    stream << "BinaryExpressionNode(" << op_ << ", " << *operand1_ << ", " << *operand2_ << ")";
}
