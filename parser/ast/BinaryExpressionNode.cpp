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
