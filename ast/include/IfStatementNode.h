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
#include "StatementNode.h"
#include <vector>

class IfStatementNode : public StatementNode {
  public:
    using StmtList = std::vector<std::unique_ptr<StatementNode>>;

  private:
    std::unique_ptr<ExpressionNode> condition_;
    StmtList thenPart_;
    StmtList elsePart_;

  public:
    IfStatementNode(const FilePos& pos, std::unique_ptr<ExpressionNode> condition);
    ~IfStatementNode() override;
    const std::unique_ptr<ExpressionNode>& getCondition() const;
    StmtList& getThenPart();
    StmtList& getElsePart();

    const StmtList& getThenPart() const;
    const StmtList& getElsePart() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
