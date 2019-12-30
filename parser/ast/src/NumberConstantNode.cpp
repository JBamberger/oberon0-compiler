#include "NumberConstantNode.h"
#include "NodeVisitor.h"
#include "Scope.h"

NumberConstantNode::NumberConstantNode(const FilePos& pos, const int value)
    : ConstantNode(NodeType::number_constant, pos, "INTEGER"), value_(value)
{
}

NumberConstantNode::~NumberConstantNode() = default;

int NumberConstantNode::getValue() const { return value_; }

void NumberConstantNode::setValue(const int value) { this->value_ = value; }

void NumberConstantNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void NumberConstantNode::print(std::ostream& stream) const
{
    stream << "NumberConstant(" << value_ << ")";
}
