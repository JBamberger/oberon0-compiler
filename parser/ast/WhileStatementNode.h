#pragma once
#include "ExpressionNode.h"
#include "StatementNode.h"
#include <vector>

class WhileStatementNode : public StatementNode {
    std::unique_ptr<ExpressionNode> condition_;
    std::unique_ptr<std::vector<std::unique_ptr<StatementNode>>> body_;

  public:
    WhileStatementNode(const FilePos& pos, std::unique_ptr<ExpressionNode> condition);
    ~WhileStatementNode() override;

    const std::unique_ptr<ExpressionNode>& getCondition() const;
    const std::unique_ptr<std::vector<std::unique_ptr<StatementNode>>>& getBody() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
