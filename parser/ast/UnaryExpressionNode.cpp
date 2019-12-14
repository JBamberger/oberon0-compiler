#include "UnaryExpressionNode.h"
#include "NodeVisitor.h"
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

void UnaryExpressionNode::print(std::ostream& stream) const
{
    stream << "UnaryExpression(" << operator_ << ", " << *operand_ << ")";
}
