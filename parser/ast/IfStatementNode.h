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
    virtual ~IfStatementNode() override;
    void setElseBody(const StatementSequenceNode* elsePart);
    void print(std::ostream& stream) const override;
};
