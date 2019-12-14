#include "NumberConstantNode.h"
#include "NodeVisitor.h"
#include <cassert>

NumberConstantNode::NumberConstantNode(const FilePos& pos, const int value)
    : ExpressionNode(NodeType::number_constant, pos), value_(value)
{
}

NumberConstantNode::~NumberConstantNode() = default;

int NumberConstantNode::getValue() const { return value_; }
void NumberConstantNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void NumberConstantNode::print(std::ostream& stream) const
{
    stream << "NumberConstant(" << value_ << ")";
}
