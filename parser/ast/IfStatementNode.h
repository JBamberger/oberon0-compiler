#pragma once
#include "ExpressionNode.h"
#include "StatementNode.h"
#include "StatementSequenceNode.h"

class IfStatementNode : public StatementNode {
    std::unique_ptr<const ExpressionNode> condition_;
    std::unique_ptr<std::vector<std::unique_ptr<StatementNode>>> thenPart_;
    std::unique_ptr<std::vector<std::unique_ptr<StatementNode>>> elsePart_;

  public:
    IfStatementNode(const FilePos& pos, const ExpressionNode* condition);
    ~IfStatementNode() override;
    const std::unique_ptr<const ExpressionNode>& getCondition() const;
    const std::unique_ptr<std::vector<std::unique_ptr<StatementNode>>>& getThenPart() const;
    const std::unique_ptr<std::vector<std::unique_ptr<StatementNode>>>& getElsePart() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
