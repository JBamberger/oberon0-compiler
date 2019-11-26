#include "UnaryExpressionNode.h"

UnaryExpressionNode::UnaryExpressionNode(const FilePos& pos,
                                         UnaryOperator operator_,
                                         const ExpressionNode* operand)
    : ExpressionNode(NodeType::unary_expression, pos), operator_(operator_), operand_(operand)
{
}

UnaryExpressionNode::~UnaryExpressionNode() = default;

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

void UnaryExpressionNode::print(std::ostream& stream) const
{
    stream << "UnaryExpression(" << operator_ << ", " << operand_ << ")";
}
