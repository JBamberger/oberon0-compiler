#pragma once
#include "ExpressionNode.h"
#include "Token.h"

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

std::ostream& operator<<(std::ostream& stream, const BinaryOperator& op);

BinaryOperator toBinaryOperator(const TokenType& type);

class BinaryExpressionNode : public ExpressionNode {
    BinaryOperator op_;
    std::unique_ptr<ExpressionNode> operand1_;
    std::unique_ptr<ExpressionNode> operand2_;

  public:
    BinaryExpressionNode(const FilePos& pos,
                         BinaryOperator op,
                         std::unique_ptr<ExpressionNode> operand1,
                         std::unique_ptr<ExpressionNode> operand2);
    ~BinaryExpressionNode() override;

    BinaryOperator getOperator() const;
    const std::unique_ptr<ExpressionNode>& getOperand1() const;
    const std::unique_ptr<ExpressionNode>& getOperand2() const;
    void visit(NodeVisitor* visitor) const override;
    void print(std::ostream& stream) const override;
};