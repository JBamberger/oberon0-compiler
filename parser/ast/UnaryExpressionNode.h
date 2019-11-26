#pragma once

#include "ExpressionNode.h"

enum class UnaryOperator : char {
    plus,
    minus,
    not
};

class UnaryExpressionNode : public ExpressionNode {
    UnaryOperator operator_;
    std::unique_ptr<const ExpressionNode> operand_;

  public:
    UnaryExpressionNode(const FilePos& pos, UnaryOperator operator_, const ExpressionNode* operand);
    virtual ~UnaryExpressionNode() override;

    void print(std::ostream& stream) const override;
};