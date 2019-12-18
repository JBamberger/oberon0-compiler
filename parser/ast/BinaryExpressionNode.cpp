#include "BinaryExpressionNode.h"
#include "NodeVisitor.h"
#include <cassert>

BinaryExpressionNode::BinaryExpressionNode(const FilePos& pos,
                                           BinaryOperator op,
                                           const ExpressionNode* operand1,
                                           const ExpressionNode* operand2)
    : ExpressionNode(NodeType::binary_expression, pos), op_(op), operand1_(operand1),
      operand2_(operand2)
{
    assert(operand1 != nullptr);
    assert(operand2 != nullptr);
}

BinaryExpressionNode::~BinaryExpressionNode() = default;

BinaryOperator BinaryExpressionNode::getOperator() const { return op_; }

const std::unique_ptr<const ExpressionNode>& BinaryExpressionNode::getOperand1() const
{
    return operand1_;
}

const std::unique_ptr<const ExpressionNode>& BinaryExpressionNode::getOperand2() const
{
    return operand2_;
}

void BinaryExpressionNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

std::ostream& operator<<(std::ostream& stream, const BinaryOperator& op)
{
    // clang-format off
    switch (op) {
    case BinaryOperator::plus:        stream << "'+'";   break;
    case BinaryOperator::minus:       stream << "'-'";   break;
    case BinaryOperator::times:       stream << "'*'";   break;
    case BinaryOperator::div:         stream << "'DIV'"; break;
    case BinaryOperator::mod:         stream << "'MOD'"; break;
    case BinaryOperator::logical_or:  stream << "'OR'";  break;
    case BinaryOperator::logical_and: stream << "'&'";   break;
    case BinaryOperator::eq:          stream << "'='";   break;
    case BinaryOperator::neq:         stream << "'#'";   break;
    case BinaryOperator::lt:          stream << "'<'";   break;
    case BinaryOperator::leq:         stream << "'<='";  break;
    case BinaryOperator::gt:          stream << "'>'";   break;
    case BinaryOperator::geq:         stream << "'>='";  break;
    default: std::terminate();
    }
    // clang-format on
    return stream;
}

BinaryOperator toBinaryOperator(const TokenType& type)
{
    switch (type) {
    case TokenType::op_eq:
        return BinaryOperator::eq;
    case TokenType::op_neq:
        return BinaryOperator::neq;
    case TokenType::op_lt:
        return BinaryOperator::lt;
    case TokenType::op_leq:
        return BinaryOperator::leq;
    case TokenType::op_gt:
        return BinaryOperator::gt;
    case TokenType::op_geq:
        return BinaryOperator::geq;
    case TokenType::op_plus:
        return BinaryOperator::plus;
    case TokenType::op_minus:
        return BinaryOperator::minus;
    case TokenType::op_or:
        return BinaryOperator::logical_or;
    case TokenType::op_times:
        return BinaryOperator::times;
    case TokenType::op_div:
        return BinaryOperator::div;
    case TokenType::op_mod:
        return BinaryOperator::mod;
    case TokenType::op_and:
        return BinaryOperator::logical_and;
    default:
        throw std::runtime_error("Invalid binary token type.");
    }
}

void BinaryExpressionNode::print(std::ostream& stream) const
{
    stream << "BinaryExpressionNode(" << op_ << ", " << *operand1_ << ", " << *operand2_ << ")";
}
