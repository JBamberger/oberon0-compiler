#pragma once
#include "ExpressionNode.h"
#include "StatementNode.h"
#include "StatementSequenceNode.h"
#include <vector>

class WhileStatementNode : public StatementNode {
    std::unique_ptr<const ExpressionNode> condition_;
    std::unique_ptr<std::vector<std::unique_ptr<StatementNode>>> body_;

  public:
    WhileStatementNode(const FilePos& pos, const ExpressionNode* condition);
    ~WhileStatementNode() override;

    const std::unique_ptr<const ExpressionNode>& getCondition() const;
    const std::unique_ptr<std::vector<std::unique_ptr<StatementNode>>>& getBody() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};
