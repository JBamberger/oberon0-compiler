#pragma once
#include "ExpressionNode.h"
#include "StatementNode.h"
#include "StatementSequenceNode.h"
#include <vector>

class WhileStatementNode : public StatementNode {
    std::unique_ptr<const ExpressionNode> condition_;
    std::unique_ptr<const StatementSequenceNode> body_;

  public:
    WhileStatementNode(const FilePos& pos,
                       const ExpressionNode* condition,
                       const StatementSequenceNode* body);
    ~WhileStatementNode() override;

    const std::unique_ptr<const ExpressionNode>& getCondition() const;
    const std::unique_ptr<const StatementSequenceNode>& getBody() const;
    void print(std::ostream& stream) const override;
};
