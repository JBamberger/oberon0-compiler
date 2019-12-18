#include "UnaryExpressionNode.h"
#include "NodeVisitor.h"
#include "Token.h"
#include <cassert>

UnaryExpressionNode::UnaryExpressionNode(const FilePos& pos,
                                         UnaryOperator op,
                                         const ExpressionNode* operand)
    : ExpressionNode(NodeType::unary_expression, pos), operator_(op), operand_(operand)
{
    assert(operand_ != nullptr);
}

UnaryExpressionNode::~UnaryExpressionNode() = default;

UnaryOperator UnaryExpressionNode::getOperator() const { return operator_; }

const std::unique_ptr<const ExpressionNode>& UnaryExpressionNode::getOperand() const
{
    return operand_;
}

void UnaryExpressionNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

std::ostream& operator<<(std::ostream& stream, const UnaryOperator& op)
{
    switch (op) {
    case UnaryOperator::plus:
        stream << "'+'";
        break;
    case UnaryOperator::minus:
        stream << "'-'";
        break;
    case UnaryOperator::not:
        stream << "'~'";
        break;
    default:
        std::terminate();
    }
    return stream;
}

UnaryOperator toUnaryOperator(const TokenType& type)
{
    switch (type) {
    case TokenType::op_plus:
        return UnaryOperator::plus;
    case TokenType::op_minus:
        return UnaryOperator::minus;
    case TokenType::op_not:
        return UnaryOperator::not;
    default:
        throw std::runtime_error("Invalid unary token type.");
    }
}

void UnaryExpressionNode::print(std::ostream& stream) const
{
    stream << "UnaryExpression(" << operator_ << ", " << *operand_ << ")";
}
