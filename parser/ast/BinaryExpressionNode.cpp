#include "BinaryExpressionNode.h"
#include "NodeVisitor.h"
#include "Scope.h"
#include <cassert>

inline std::string selectType(const BinaryOperator op)
{
    switch (op) {
    case BinaryOperator::plus:
    case BinaryOperator::minus:
    case BinaryOperator::times:
    case BinaryOperator::div:
    case BinaryOperator::mod:
        return "INTEGER";
    case BinaryOperator::logical_or:
    case BinaryOperator::logical_and:
    case BinaryOperator::eq:
    case BinaryOperator::neq:
    case BinaryOperator::lt:
    case BinaryOperator::leq:
    case BinaryOperator::gt:
    case BinaryOperator::geq:
        return "BOOLEAN";
    default:
        std::terminate();
    }
}

BinaryExpressionNode::BinaryExpressionNode(const FilePos& pos,
                                           const BinaryOperator op,
                                           std::unique_ptr<ExpressionNode> operand1,
                                           std::unique_ptr<ExpressionNode> operand2)
    : ExpressionNode(NodeType::binary_expression, pos, selectType(op)), op_(op),
      operand1_(std::move(operand1)), operand2_(std::move(operand2))
{
    assert(operand1_ != nullptr);
    assert(operand2_ != nullptr);
    assert(operand1_->getType() == operand2_->getType());
}

BinaryExpressionNode::~BinaryExpressionNode() = default;

BinaryOperator BinaryExpressionNode::getOperator() const { return op_; }

const std::unique_ptr<ExpressionNode>& BinaryExpressionNode::getOperand1() const
{
    return operand1_;
}

const std::unique_ptr<ExpressionNode>& BinaryExpressionNode::getOperand2() const
{
    return operand2_;
}

void BinaryExpressionNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void BinaryExpressionNode::print(std::ostream& stream) const
{
    stream << "BinaryExpressionNode(" << op_ << ", " << *operand1_ << ", " << *operand2_ << ")";
}

int BinaryExpressionNode::eval(BinaryOperator op, int v1, int v2)
{
    switch (op) {
    case BinaryOperator::times:
        return v1 * v2;
    case BinaryOperator::div:
        return v1 / v2;
    case BinaryOperator::mod:
        return v1 % v2;
    case BinaryOperator::plus:
        return v1 + v2;
    case BinaryOperator::minus:
        return v1 - v2;
    case BinaryOperator::logical_and:
        return v1 && v2;
    case BinaryOperator::logical_or:
        return v1 || v2;
    case BinaryOperator::eq:
        return v1 == v2;
    case BinaryOperator::neq:
        return v1 != v2;
    case BinaryOperator::lt:
        return v1 < v2;
    case BinaryOperator::leq:
        return v1 <= v2;
    case BinaryOperator::gt:
        return v1 > v2;
    case BinaryOperator::geq:
        return v1 >= v2;
    default:
        std::terminate();
    }
}
