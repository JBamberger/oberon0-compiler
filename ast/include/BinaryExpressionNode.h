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

#pragma once
#include "ExpressionNode.h"
#include "Operator.h"
#include "Token.h"

class BinaryExpressionNode : public ExpressionNode {
    BinaryOperator op_;
    std::unique_ptr<ExpressionNode> operand1_;
    std::unique_ptr<ExpressionNode> operand2_;

  public:
    BinaryExpressionNode(const FilePos& pos,
                         TypeNode* type,
                         BinaryOperator op,
                         std::unique_ptr<ExpressionNode> operand1,
                         std::unique_ptr<ExpressionNode> operand2);
    ~BinaryExpressionNode() override;

    BinaryOperator getOperator() const;
    const std::unique_ptr<ExpressionNode>& getOperand1() const;
    const std::unique_ptr<ExpressionNode>& getOperand2() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
