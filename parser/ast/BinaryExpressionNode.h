#pragma once
#include "ExpressionNode.h"

enum class BinaryOperator : char {
    plus,
    minus,
    times,
    div,
    mod,
    logical_or,
    logical_and,
    eq,
    neq,
    lt,
    leq,
    gt,
    geq
};

class BinaryExpressionNode : public ExpressionNode {
    BinaryOperator op_;
    std::unique_ptr<const ExpressionNode> operand1_;
    std::unique_ptr<const ExpressionNode> operand2_;

  public:
    BinaryExpressionNode(const FilePos& pos,
                         BinaryOperator op,
                         const ExpressionNode* operand1,
                         const ExpressionNode* operand2);
    virtual ~BinaryExpressionNode() override;

    void print(std::ostream& stream) const override;
};