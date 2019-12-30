#pragma once

#include "ExpressionNode.h"
#include "Operator.h"

class UnaryExpressionNode : public ExpressionNode {
    UnaryOperator operator_;
    std::unique_ptr<ExpressionNode> operand_;

  public:
    UnaryExpressionNode(const FilePos& pos, UnaryOperator op, std::unique_ptr<ExpressionNode> operand);
    ~UnaryExpressionNode() override;

    UnaryOperator getOperator() const;
    const std::unique_ptr<ExpressionNode>& getOperand() const;

    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};