#include "UnaryExpressionNode.h"
#include "NodeVisitor.h"
#include "Scope.h"
#include "Token.h"
#include <cassert>

inline std::shared_ptr<TypeNode> selectType(const UnaryOperator op)
{
    switch (op) {
    case UnaryOperator::plus:
    case UnaryOperator::minus:
        return Scope::INTEGER;
    case UnaryOperator::not:
        return Scope::BOOLEAN;
    default:
        std::terminate();
    }
}

UnaryExpressionNode::UnaryExpressionNode(const FilePos& pos,
                                         const UnaryOperator op,
                                         std::unique_ptr<ExpressionNode> operand)
    : ExpressionNode(NodeType::unary_expression, pos, selectType(op)), operator_(op),
      operand_(std::move(operand))
{
    assert(operand_ != nullptr);
}

UnaryExpressionNode::~UnaryExpressionNode() = default;

UnaryOperator UnaryExpressionNode::getOperator() const { return operator_; }

const std::unique_ptr<ExpressionNode>& UnaryExpressionNode::getOperand() const { return operand_; }

void UnaryExpressionNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void UnaryExpressionNode::print(std::ostream& stream) const
{
    stream << "UnaryExpression(" << operator_ << ", " << *operand_ << ")";
}
