#include "BinaryExpressionNode.h"
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

void BinaryExpressionNode::print(std::ostream& stream) const
{
    stream << "BinaryExpressionNode(" << op_ << ", " << *operand1_ << ", " << *operand2_ << ")";
}
