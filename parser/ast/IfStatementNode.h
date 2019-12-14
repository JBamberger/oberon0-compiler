#pragma once
#include "ExpressionNode.h"
#include "StatementNode.h"
#include "StatementSequenceNode.h"

class IfStatementNode : public StatementNode {
    std::unique_ptr<const ExpressionNode> condition_;
    std::unique_ptr<const StatementSequenceNode> thenPart_;
    std::unique_ptr<const StatementSequenceNode> elsePart_;

  public:
    IfStatementNode(const FilePos& pos,
                    const ExpressionNode* condition,
                    const StatementSequenceNode* thenPart);
    ~IfStatementNode() override;
    void setElseBody(const StatementSequenceNode* elsePart);
    const std::unique_ptr<const ExpressionNode>& getCondition() const;
    const std::unique_ptr<const StatementSequenceNode>& getThenPart() const;
    const std::unique_ptr<const StatementSequenceNode>& getElsePart() const;
    void print(std::ostream& stream) const override;
};
