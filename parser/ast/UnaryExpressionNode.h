#pragma once

#include "ExpressionNode.h"

enum class UnaryOperator : char { plus, minus, not};

std::ostream& operator<<(std::ostream& out, const UnaryOperator& op);

class UnaryExpressionNode : public ExpressionNode {
    UnaryOperator operator_;
    std::unique_ptr<const ExpressionNode> operand_;

  public:
    UnaryExpressionNode(const FilePos& pos, UnaryOperator op, const ExpressionNode* operand);
    ~UnaryExpressionNode() override;

    UnaryOperator getOperator() const;
    const std::unique_ptr<const ExpressionNode>& getOperand() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};