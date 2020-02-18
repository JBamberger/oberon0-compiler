#include "NumberConstantNode.h"
#include "NodeVisitor.h"
#include "Scope.h"
#include <cassert>

NumberConstantNode::NumberConstantNode(const FilePos& pos, const int value, TypeNode* type)
    : ConstantNode(pos, type), value_(value)
{
    assert(type->getId() == "INTEGER");
}

NumberConstantNode::~NumberConstantNode() = default;

int NumberConstantNode::getValue() const
{
    return value_;
}

void NumberConstantNode::setValue(const int value)
{
    this->value_ = value;
}

void NumberConstantNode::visit(NodeVisitor* visitor) const
{
    visitor->visit(this);
}

void NumberConstantNode::print(std::ostream& stream) const
{
    stream << "NumberConstant(" << value_ << ")";
}
